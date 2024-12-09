#include "AIRLController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "../Player/PlayerRLState.h"
#include "../Piece/Piece.h"
#include "../Player/PlayerCharacter.h"
#include "../../RLHighLevel/RoundManager.h"
#include "../../RLHighLevel/MapManager.h"

#include "../Environment/EnvBoard.h"
#include "../Environment/EnvSquare.h"
#include "../Environment/EnvShop.h"

AAIRLController::AAIRLController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAIRLController::BeginPlay()
{
    Super::BeginPlay();
}

FString AAIRLController::getPlayerName()
{
    return rlPlayerState->playerName;
}
FColor AAIRLController::getPlayerColor()
{
    return rlPlayerState->playerColor;
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

void AAIRLController::setCurrentValueMap()
{
    valueMap.Empty();

    TArray<APiece*> allFriendlyPieces = controlledPlayer->getArmy();
    AEnvBoard* gameBoard = UMapManager::get()->getGameBoard();

    for (APiece* eachPiece : allFriendlyPieces)
    {
        if (eachPiece)
        {
            TArray<FVector2D> allPossibleMoves = eachPiece->calculatePossibleMove();
            AEnvSquare* curSquare = nullptr;
            for (const FVector2D& eachLocation : allPossibleMoves)
            {
                curSquare = gameBoard->getSquareAtLocation(eachLocation);
                if (curSquare->getIsOccupied())
                {
                    APiece* occupiedPiece = curSquare->getOccupiedPiece();
                    if (occupiedPiece)
                    {
                        if (occupiedPiece->getPieceColor() != controlledPlayer->getPlayerColor())
                        {
                            valueMap.FindOrAdd(eachLocation) += 5;
                        }
                        else
                        {
                            valueMap.FindOrAdd(eachLocation) -= 1;
                        }
                    }
                }
                else
                {
                    valueMap.FindOrAdd(eachLocation) += 1;
                }
            }
        }
    }
}

void AAIRLController::makeTheBestMove()
{
    FVector2D curBestMove = selectBestMove();
    TArray<APiece*> allFriendlyPieces = controlledPlayer->getArmy();
    AEnvBoard* gameBoard = UMapManager::get()->getGameBoard();
    for (APiece* eachPiece : allFriendlyPieces)
    {
        TArray<FVector2D> allPossibleMoves = eachPiece->calculatePossibleMove();
        if (allPossibleMoves.Contains(curBestMove))
        {
            controlledPlayer->setSelectedPiece(eachPiece);
            controlledPlayer->setSelectedSquare(gameBoard->getSquareAtLocation(curBestMove));
            controlledPlayer->moveSelectedPiece();
            break;
        }
    }
}

FVector2D AAIRLController::selectBestMove()
{
    FVector2D BestMove;
    int32 HighestValue = INT_MIN;

    for (const auto& Elem : valueMap)
    {
        if (Elem.Value > HighestValue)
        {
            HighestValue = Elem.Value;
            BestMove = Elem.Key;
        }
    }

    return BestMove;
}

void AAIRLController::MoveToRandomLocation()
{
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
    if (NavSys)
    {
        FVector MyLocation = GetPawn()->GetActorLocation();
        FNavLocation RandomLocation;

        // Get a random point on the navigation mesh
        if (NavSys->GetRandomPointInNavigableRadius(MyLocation, findRaidus, RandomLocation))
        {
            // Use the built-in AI move to function
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation.Location);
        }
    }
}