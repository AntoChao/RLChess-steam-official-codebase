#include "PieceCowboy.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Environment/EnvBoard.h"

APieceCowboy::APieceCowboy()
{
}

TArray<FVector2D> APieceCowboy::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the cowboy is on the board
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

        // Process each direction for free movement and obstacle detection
        for (const EPieceDirection& Direction : Directions)
        {
            // Free movement within 2 squares
            TArray<FVector2D> LineMoves = getLineMoveWithNoObstacle(CurrentLocation, Direction, movePoint);
            PossibleMoves.Append(LineMoves);

            // Detect the first obstacle in the same directions beyond the free movement
            TArray<FVector2D> LineWithFirstObstacle = getLineMoveWithFirstObstacle(CurrentLocation, Direction, movePoint * 2);
            if (LineWithFirstObstacle.Num() > movePoint && LineWithFirstObstacle.Num() <= movePoint * 2)
            {
                // POSSIBLE ERROR
                specialPossibleMove.Add(LineWithFirstObstacle[LineWithFirstObstacle.Num() - 1]);
            }
        }
    }

    return PossibleMoves;
}

void APieceCowboy::bePlacedInBoardEffect_Implementation(AEnvSquare* squareDestination)
{
    if (specialPossibleMove.Contains(squareDestination->getSquareLocation()))
    {
        bePlacedSpecialSquareEffect(squareDestination);
    }
    else
    {
        startMoving(squareDestination);
    }
}

void APieceCowboy::bePlacedSpecialSquareEffect_Implementation(AEnvSquare* squareDestination)
{
    FVector2D nextVector = getDirectionVector(lastMoveDirection);
    FVector2D nextLocation = curSquare->getSquareLocation() + nextVector;

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
        AEnvSquare* nextSquare = gameBoard->getSquareAtLocation(nextLocation);

        if (squareDestination && squareDestination->getIsOccupied())
        {
            APiece* targetPiece = squareDestination->getOccupiedPiece();
            targetPiece->startMoving(nextSquare);
        }
    }
}