#include "PieceCatapult.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceCatapult::APieceCatapult()
{
}

TArray<FVector2D> APieceCatapult::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the catapult is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* GameBoard = UMapManager::get()->getGameBoard();
    if (GameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();

        TArray<FVector2D> AdjacentDirections = {
            FVector2D(-1, 0), FVector2D(1, 0),  // Up, Down
            FVector2D(0, -1), FVector2D(0, 1),  // Left, Right
            FVector2D(-1, -1), FVector2D(-1, 1), // UpLeft, UpRight
            FVector2D(1, -1), FVector2D(1, 1)   // DownLeft, DownRight
        };

        for (const FVector2D& Direction : AdjacentDirections)
        {
            FVector2D PotentialLocation = CurrentLocation + Direction;

            // Check if the potential location is valid and occupied
            if (GameBoard->isValidLocation(PotentialLocation) && GameBoard->isSquareOccupied(PotentialLocation))
            {
                PossibleMoves.Add(PotentialLocation);
            }
        }
    }

    return PossibleMoves;
}
