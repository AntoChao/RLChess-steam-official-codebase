#include "PieceAssassin.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceAssassin::APieceAssassin()
{
}

TArray<FVector2D> APieceAssassin::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the assassin is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* GameBoard = UMapManager::get()->getGameBoard();
    if (GameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();
        int BoardWidth = GameBoard->getRowSize();
        int BoardHeight = GameBoard->getColumnSize();
        int moveDistance = this->movePoint;  // Assuming movePoint is an integer property of APiece

        TArray<FVector2D> Directions = {
            FVector2D(-1, 0), FVector2D(1, 0), // Up, Down
            FVector2D(0, -1), FVector2D(0, 1), // Left, Right
            FVector2D(-1, -1), FVector2D(-1, 1), // UpLeft, UpRight
            FVector2D(1, -1), FVector2D(1, 1)  // DownLeft, DownRight
        };

        for (const FVector2D& Direction : Directions)
        {
            for (int step = 1; step <= moveDistance; ++step)
            {
                FVector2D PotentialLocation = CurrentLocation + Direction * step;

                // Wrap around the board if the location is out of bounds
                int newX = FMath::FloorToInt(PotentialLocation.X + BoardHeight) % BoardHeight;
                int newY = FMath::FloorToInt(PotentialLocation.Y + BoardWidth) % BoardWidth;

                PotentialLocation = FVector2D(newX, newY);

                if (GameBoard->isValidLocation(PotentialLocation))
                {
                    PossibleMoves.Add(PotentialLocation);
                }
            }
        }
    }

    return PossibleMoves;
}