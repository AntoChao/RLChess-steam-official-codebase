#include "PieceKnight.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceKnight::APieceKnight()
{
}

TArray<FVector2D> APieceKnight::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the knight is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* GameBoard = UMapManager::get()->getGameBoard();
    if (GameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();

        int Distance = movePoint + 2;  // Calculate the exact distance for the moves based on movePoint

        // Generate all potential moves within the calculated distance
        for (int dx = -Distance; dx <= Distance; ++dx)
        {
            for (int dy = -Distance; dy <= Distance; ++dy)
            {
                if (dx == 0 || dy == 0 || FMath::Abs(dx) == FMath::Abs(dy))  // Skip straight line moves
                {
                    continue;
                }

                int sumOfSteps = FMath::Abs(dx) + FMath::Abs(dy);
                if (sumOfSteps != Distance)  // Only allow moves that equal the exact distance
                {
                    continue;
                }

                FVector2D PotentialLocation = CurrentLocation + FVector2D(dx, dy);
                // Ensure the potential move is within the game board limits and is either unoccupied or occupied by an opponent's piece
                if (GameBoard->isValidLocation(PotentialLocation))
                {
                    PossibleMoves.Add(PotentialLocation);  // Add the move regardless of occupancy
                }
            }
        }
    }

    return PossibleMoves;
}