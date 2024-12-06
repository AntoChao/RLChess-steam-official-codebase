#include "PieceWarrior.h"

#include "../../RLHighLevel/GameplayGameMode.h"

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

    AEnvBoard* GameBoard = UMapManager::get()->getGameBoard();
    
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