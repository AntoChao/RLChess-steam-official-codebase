#include "PieceRanger.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

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

    AEnvBoard* GameBoard = nullptr;
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            GameBoard = GameState->getGameBoard();
        }
    }

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

void APieceRanger::killEffect_Implementation()
{
    moveMode = EPieceMoveMode::EStaticJump;

    isKillEffectActive = false;
    EPieceDirection backDirection = getOppositeDirection(lastMoveDirection);
    FVector2D backVector = getDirectionVector(backDirection);
    FVector2D backLocation = curSquare->getSquareLocation() + backVector;

    AEnvBoard* gameBoard = nullptr;
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            gameBoard = GameState->getGameBoard();
        }
    }

    if (gameBoard)
    {
        AEnvSquare* backSquare = gameBoard->getSquareAtLocation(backLocation);

        startMoving(backSquare);
    }
}