#include "PieceCowboy.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceCowboy::APieceCowboy()
{
}

TArray<FVector2D> APieceCowboy::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the cowboy is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* GameBoard = UMapManager::get()->getGameBoard(); 
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
            TArray<FVector2D> LineMoves = getLineMoveWithNoObstacle(CurrentLocation, Direction, movePoint);
            PossibleMoves.Append(LineMoves);

            // Detect the first obstacle in the same directions beyond the free movement
            TArray<FVector2D> LineWithFirstObstacle = getLineMoveWithFirstObstacle(CurrentLocation, Direction, INT_MAX);
            if (!LineWithFirstObstacle.IsEmpty())
            {
                // Only add the position of the first obstacle, which is the first element after the free moves
                if (LineWithFirstObstacle.Num() > LineMoves.Num())  // Ensure there is an obstacle beyond free moves
                {
                    PossibleMoves.Add(LineWithFirstObstacle[LineMoves.Num()]);  // The first obstacle after the last free move
                }
            }
        }
    }

    return PossibleMoves;
}