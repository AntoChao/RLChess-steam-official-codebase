#include "PieceRanger.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceRanger::APieceRanger()
{
}

TArray<FVector2D> APieceRanger::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare)
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

        for (EPieceDirection Direction : Directions)
        {
            TArray<FVector2D> DiagonalMoves = getLineMoveWithFirstObstacle(CurrentLocation, Direction, movePoint);
            PossibleMoves.Append(DiagonalMoves);
        }
    }

    return PossibleMoves;
}