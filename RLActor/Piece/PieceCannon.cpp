#include "PieceCannon.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceCannon::APieceCannon()
{
}

TArray<FVector2D> APieceCannon::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the cannon is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* GameBoard = UMapManager::get()->getGameBoard();
    if (GameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();

        // Directions cannon can move: up, down, left, right
        TArray<EPieceDirection> Directions = {
            EPieceDirection::EUp, EPieceDirection::EDown,
            EPieceDirection::ELeft, EPieceDirection::ERight
        };

        // Iterate over each direction for movement and capturing
        for (EPieceDirection Direction : Directions)
        {
            // First, add all possible moves until the first obstacle or end of board
            TArray<FVector2D> LineMovesNoObstacle = getLineMoveWithNoObstacle(CurrentLocation, Direction, INT_MAX);
            PossibleMoves.Append(LineMovesNoObstacle);

            // Then, find the first obstacle and any potential capture targets beyond it
            TArray<FVector2D> FullLineMoves = getLineMove(CurrentLocation, Direction, INT_MAX);
            bool foundObstacle = false;
            FVector2D obstaclePosition;
            for (const FVector2D& Pos : FullLineMoves)
            {
                if (GameBoard->isSquareOccupied(Pos))
                {
                    if (!foundObstacle)
                    {
                        foundObstacle = true;
                        obstaclePosition = Pos;
                        continue; // Continue to find a capture target beyond this obstacle
                    }
                    else
                    {
                        // Check for capture target beyond the first obstacle
                        if (Pos != obstaclePosition)
                        {
                            PossibleMoves.Add(Pos);
                            break; // Stop after the first valid capture position to mimic jumping over one piece
                        }
                    }
                }
            }
        }
    }

    return PossibleMoves;
}