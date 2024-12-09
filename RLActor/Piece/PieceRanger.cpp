#include "PieceRanger.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceRanger::APieceRanger()
{
}

TArray<FVector2D> APieceRanger::calculatePossibleMove()
{
    isKillEffectActive = true;
    TArray<FVector2D> PossibleMoves;
    moveMode = EPieceMoveMode::EGround;
    
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
            TArray<FVector2D> LineMoves = getLineMoveWithFirstObstacle(CurrentLocation, Direction, movePoint);
            PossibleMoves.Append(LineMoves);
        }
    }

    return PossibleMoves;
}

void APieceRanger::killEffect()
{
    moveMode = EPieceMoveMode::EStaticJump;

    isKillEffectActive = false;
    EPieceDirection backDirection = getOppositeDirection(lastMoveDirection);
    FVector2D backVector = getDirectionVector(backDirection);
    FVector2D backLocation = curSquare->getSquareLocation() + backVector;

    UMapManager* mapManager = UMapManager::get();
    AEnvBoard* gameBoard = mapManager->getGameBoard();

    if (gameBoard)
    {
        AEnvSquare* backSquare = gameBoard->getSquareAtLocation(backLocation);

        startMoving(backSquare);
    }
}