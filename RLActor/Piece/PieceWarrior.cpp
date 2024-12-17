#include "PieceWarrior.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Environment/EnvBoard.h"

APieceWarrior::APieceWarrior()
{
}

TArray<FVector2D> APieceWarrior::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the warrior is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* GameBoard = nullptr;
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            GameBoard = GameState->getGameBoard();
        }
    }

    
    if (GameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();
        int Distance = movePoint;  // The exact distance the warrior can move

        TArray<FVector2D> Directions = {
            FVector2D(-Distance, 0), 
            FVector2D(Distance, 0), 
            FVector2D(0, -Distance),
            FVector2D(0, Distance) 
        };

        for (const FVector2D& Direction : Directions)
        {
            FVector2D PotentialLocation = CurrentLocation + Direction;
            if (GameBoard->isValidLocation(PotentialLocation) && !GameBoard->isSquareOccupied(PotentialLocation))
            {
                PossibleMoves.Add(PotentialLocation);
            }
        }
    }

    return PossibleMoves;
}

void APieceWarrior::endMoving_Implementation()
{
    Super::endMoving();

    AEnvBoard* GameBoard = nullptr;
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            GameBoard = GameState->getGameBoard();
        }
    }

    if (GameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();
        TArray<EPieceDirection> Directions = {
            EPieceDirection::EUp, EPieceDirection::EDown,
            EPieceDirection::ELeft, EPieceDirection::ERight
        };

        // Process each direction for free movement and obstacle detection
        for (const EPieceDirection& Direction : Directions)
        {
            // Free movement within 2 squares
            TArray<FVector2D> LineMoves = getLineMoveWithFirstObstacle(CurrentLocation, Direction, 1);

            for (FVector2D locToCheck : LineMoves)
            {
                if (GameBoard->isSquareOccupied(locToCheck))
                {
                    APiece* checkPiece = GameBoard->getPieceAtLocation(locToCheck);
                    if (checkPiece)
                    {
                        kill(checkPiece);
                    }
                }
            }
        }
    }
}