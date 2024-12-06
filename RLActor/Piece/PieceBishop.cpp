#include "PieceBishop.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceBishop::APieceBishop()
{
}

TArray<FVector2D> APieceBishop::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the bishop is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* GameBoard = UMapManager::get()->getGameBoard();
    if (GameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();

        // Directions bishop can move: all four diagonal directions
        TArray<EPieceDirection> Directions = {
            EPieceDirection::EUpRight, EPieceDirection::EUpLeft,
            EPieceDirection::EDownRight, EPieceDirection::EDownLeft
        };

        // Iterate over each diagonal direction and calculate possible moves
        for (EPieceDirection Direction : Directions)
        {
            TArray<FVector2D> DiagonalMoves = getLineMoveWithFirstObstacle(CurrentLocation, Direction, INT_MAX); // INT_MAX to simulate unlimited range
            PossibleMoves.Append(DiagonalMoves);
        }
    }

    return PossibleMoves;
}