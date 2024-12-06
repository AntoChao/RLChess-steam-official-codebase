#include "PieceRook.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceRook::APieceRook()
{
}

TArray<FVector2D> APieceRook::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the rook is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* gameBoard = UMapManager::get()->getGameBoard();
    if (gameBoard)
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
            TArray<FVector2D> LineMoves = getLineMoveWithFirstObstacle(CurrentLocation, Direction, INT_MAX); // INT_MAX to simulate unlimited range
            PossibleMoves.Append(LineMoves);
        }
    }

    return PossibleMoves;
}