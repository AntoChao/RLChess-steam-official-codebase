#include "AIRLController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "Net/UnrealNetwork.h"

#include "../Player/PlayerRLState.h"
#include "../Piece/Piece.h"
#include "../Player/PlayerCharacter.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Environment/EnvBoard.h"
#include "../Environment/EnvSquare.h"
#include "../Environment/EnvShop.h"

#include "SimulatedBoardState.h"

AAIRLController::AAIRLController()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
}

void AAIRLController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AAIRLController, aiControllerIndex);
    DOREPLIFETIME(AAIRLController, isDied);
}

void AAIRLController::BeginPlay()
{
    Super::BeginPlay();

    aiBoardState = NewObject<USimulatedBoardState>();
}

void AAIRLController::gameStateCreateBody_Implementation()
{
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            // Successfully cast to MyCustomGameMode, now you can access its members
            GameState->createPlayerBody(isDied, aiControllerIndex);

            serverPossesses(this);
        }
    }
}
void AAIRLController::serverPossesses_Implementation(AAIRLController* currentController)
{
    APawn* controlledPawn = currentController->GetPawn();

    if (IsValid(controlledPawn))
    {
        controlledPawn->Destroy();
    }

    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            // Successfully cast to MyCustomGameMode, now you can access its members
            rlPlayerBody = GameState->getPlayerBody(aiControllerIndex);
            GameState->notifyBodyCreation();
            currentController->Possess(rlPlayerBody);
        }
    }
}

void AAIRLController::setIndex_Implementation(int controllerIndex)
{
    aiControllerIndex = controllerIndex;
}

FString AAIRLController::getPlayerName()
{
    return TEXT("AI");
}
void AAIRLController::setAIPlayerColor(FColor aColor)
{
    AIColor = aColor;
}
FColor AAIRLController::getPlayerColor()
{
    return AIColor;
}
FString AAIRLController::getCharacterName()
{
    if (!rlPlayerState)
    {
        rlPlayerState = GetPlayerState<APlayerRLState>();
    }
    return rlPlayerState->characterName;
}

void AAIRLController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}
FMove AAIRLController::selectBestMove(TArray<APiece*> army)
{
    TArray<AActor*> FoundActors;
    UWorld* World = GetWorld();
    FMove BestMove;

    if (World)
    {
        UGameplayStatics::GetAllActorsOfClass(World, APiece::StaticClass(), FoundActors);

        TArray<APiece*> opponentsPieces;
        for (AActor* eachActor : FoundActors)
        {
            APiece* eachPiece = Cast<APiece>(eachActor);
            if (eachPiece)
            {
                if (!army.Contains(eachPiece))
                {
                    opponentsPieces.Add(eachPiece);
                }
            }
        }
        synchronizeBoardState(army, opponentsPieces);

        int32 Alpha = INT32_MIN;
        int32 Beta = INT32_MAX;
        

        Minimax(aiBoardState, DepthMiniMax, Alpha, Beta, true, BestMove);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Miau"));

    }
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Wang"));

    return BestMove;
}

void AAIRLController::synchronizeBoardState(TArray<APiece*> myPieces, TArray<APiece*> opponentPieces)
{
    if (aiBoardState)
    {
        aiBoardState->PlayerPieces.Empty();
        aiBoardState->OpponentPieces.Empty();

        AEnvSquare* tempSquare;
        for (APiece* Piece : myPieces) {
            if (Piece)
            {
                tempSquare = Piece->getOccupiedSquare();
                if (tempSquare)
                {
                    FVector2D PieceLocation = tempSquare->getSquareLocation();
                    aiBoardState->PlayerPieces.Add(FSimulatedPiece(Piece, PieceLocation, true));
                }

            }
        }

        for (APiece* Piece : opponentPieces) {
            if (Piece)
            {
                tempSquare = Piece->getOccupiedSquare();
                if (tempSquare)
                {
                    FVector2D PieceLocation = tempSquare->getSquareLocation();
                    aiBoardState->OpponentPieces.Add(FSimulatedPiece(Piece, PieceLocation, false));
                }
            }
        }
    }
}

int32 AAIRLController::Minimax(USimulatedBoardState* GameState, int32 Depth, 
    int32 Alpha, int32 Beta, bool bIsAITurn, FMove& OutBestMove) {
    if (Depth == 0 || GameState->IsGameOver()) {
        return GameState->EvaluateState();
    }

    if (bIsAITurn) {
        int32 MaxEval = INT32_MIN;
        FMove BestMoveForThisLevel;

        for (FSimulatedPiece& SimPiece : GameState->PlayerPieces) {
            TArray<FVector2D> Moves = SimPiece.Piece->calculatePossibleMove();

            for (const FVector2D& Move : Moves) {
                // Create a new state for simulation
                USimulatedBoardState* NewState = NewObject<USimulatedBoardState>();
                NewState->PlayerPieces = GameState->PlayerPieces;
                NewState->OpponentPieces = GameState->OpponentPieces;

                // Apply the move and resolve conflicts
                NewState->ApplyMove(SimPiece, Move);
                NewState->ResolveConflicts();

                // Recursive call for the opponent's turn
                FMove TempMove;
                int32 Eval = Minimax(NewState, Depth - 1, Alpha, Beta, false, TempMove);

                // Track the best move for the AI
                if (Eval > MaxEval) {
                    MaxEval = Eval;
                    BestMoveForThisLevel.Piece = SimPiece.Piece;
                    BestMoveForThisLevel.Target = Move;
                }

                Alpha = FMath::Max(Alpha, Eval);
                if (Beta <= Alpha) {
                    break; // Prune
                }
            }
        }

        // Assign the best move found at this level to OutBestMove
        OutBestMove = BestMoveForThisLevel;
        return MaxEval;

    }
    else {
        int32 MinEval = INT32_MAX;
        FMove BestMoveForThisLevel;

        for (FSimulatedPiece& SimPiece : GameState->OpponentPieces) {
            TArray<FVector2D> Moves = SimPiece.Piece->calculatePossibleMove();

            for (const FVector2D& Move : Moves) {
                // Create a new state for simulation
                USimulatedBoardState* NewState = NewObject<USimulatedBoardState>();
                // Swap roles: Opponent becomes Player and vice versa
                NewState->PlayerPieces = GameState->OpponentPieces;
                NewState->OpponentPieces = GameState->PlayerPieces;

                // Apply the move and resolve conflicts
                NewState->ApplyMove(SimPiece, Move);
                NewState->ResolveConflicts();

                // Recursive call for the AI's turn
                FMove TempMove;
                int32 Eval = Minimax(NewState, Depth - 1, Alpha, Beta, true, TempMove);

                // Track the worst move for the AI (best for the opponent)
                if (Eval < MinEval) {
                    MinEval = Eval;
                    BestMoveForThisLevel.Piece = SimPiece.Piece;
                    BestMoveForThisLevel.Target = Move;
                }

                Beta = FMath::Min(Beta, Eval);
                if (Beta <= Alpha) {
                    break; // Prune
                }
            }
        }

        // Assign the best move found at this level to OutBestMove
        OutBestMove = BestMoveForThisLevel;
        return MinEval;
    }
}