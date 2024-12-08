#include "PieceCultist.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceCultist::APieceCultist()
{
}

TArray<FVector2D> APieceCultist::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the rook is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* GameBoard = UMapManager::get()->getGameBoard();
    if (GameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();

        // Directions rook can move: up, down, left, right
        TArray<EPieceDirection> Directions = {
            EPieceDirection::EUp, EPieceDirection::EDown,
            EPieceDirection::ELeft, EPieceDirection::ERight
        };

        // Iterate over each direction and calculate possible moves
        for (EPieceDirection Direction : Directions)
        {
            TArray<FVector2D> LineMoves = getLineMoveWithNoObstacle(CurrentLocation, Direction, movePoint);
            PossibleMoves.Append(LineMoves);
        }
    }

    return PossibleMoves;
}

void APieceCultist::dieEffect()
{
    Super::dieEffect();
    beExploted();
}