#include "PiecePawn.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APiecePawn::APiecePawn()
{
}

TArray<FVector2D> APiecePawn::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the pawn is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* gameBoard = UMapManager::get()->getGameBoard();
    if (gameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();

        // Get the forward direction based on the pawn's current facing
        FVector2D ForwardDirection = getDirectionVector(pieceDirection);

        // Forward movement up to movePoint spaces, handling initial move scenarios
        int curMovePoint = movePoint;
        if (!bHasMoved)
        {
            curMovePoint = curMovePoint + 1;
        }

        TArray<FVector2D> ForwardMoves = getLineMoveWithFirstObstacle(CurrentLocation, pieceDirection, curMovePoint);
        PossibleMoves.Append(ForwardMoves);

        // Diagonal capturing: Check each diagonal direction for potential captures
        TArray<FVector2D> DiagonalDirections = getDiagonalDirections();
        for (const FVector2D& DiagonalDirection : DiagonalDirections)
        {
            TArray<FVector2D> DiagonalMoves = getLineMoveWithFirstObstacle(CurrentLocation, directionVectorToEnum(DiagonalDirection), 1);
            for (FVector2D DiagonalMove : DiagonalMoves)
            {
                if (gameBoard->isSquareOccupied(DiagonalMove))
                {
                    PossibleMoves.Add(DiagonalMove);
                }
            }
        }
    }

    return PossibleMoves;
}