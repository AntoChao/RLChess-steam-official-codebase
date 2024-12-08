#include "PieceWitch.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceWitch::APieceWitch()
{
}

TArray<FVector2D> APieceWitch::calculatePossibleMove()
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
            TArray<FVector2D> LineMoves = getLineMove(CurrentLocation, Direction, INT_MAX);
            PossibleMoves.Append(LineMoves);
        }
    }

    return PossibleMoves;
}

void APieceWitch::startMoving(AEnvSquare* squareDestination)
{
    if (IsValid(curSquare))
    {
        curSquare->occupiedPieceLeaved();
    }

    FVector2D curLocation = curSquare->getSquareLocation();

    lastMoveDirection = calculateMovingDirection(squareDestination);
    FVector2D directionVector = getDirectionVector(lastMoveDirection);

    TArray<FVector2D> LineMoves = getLineMove(curLocation, lastMoveDirection, INT_MAX);
    int randomInt = FMath::RandRange(1, LineMoves.Num());

    FVector2D endVector = curLocation + (directionVector * randomInt);

    AEnvBoard* gameBoard = UMapManager::get()->getGameBoard();
    if (gameBoard)
    {
        squareDestination = gameBoard->getSquareAtLocation(endVector); // Return empty array if board is not found
        
        isMoving = true;
        isKilledAnyActorThisTurn = false;
        startLocation = GetActorLocation();
        endLocation = squareDestination->getPlacementLocation();
        targetSquare = squareDestination;

        moveBasedOnMove(squareDestination);
    }
}