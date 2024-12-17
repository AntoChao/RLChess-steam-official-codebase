#include "PieceCannon.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Environment/EnvBoard.h"

APieceCannon::APieceCannon()
{
}

TArray<FVector2D> APieceCannon::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;
    specialPossibleMove.Empty();
    moveMode = EPieceMoveMode::EGround;

    if (!curSquare) // Ensure the cannon is on the board
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

        // Directions cannon can move: up, down, left, right
        TArray<EPieceDirection> Directions = {
            EPieceDirection::EUp, EPieceDirection::EDown,
            EPieceDirection::ELeft, EPieceDirection::ERight
        };

        // Iterate over each direction for movement and capturing
        for (EPieceDirection Direction : Directions)
        {
            bool foundObstacle = false;
            TArray<FVector2D> FullLineMoves = getLineMove(CurrentLocation, Direction, INT_MAX);
            for (const FVector2D& Pos : FullLineMoves)
            {
                if (!foundObstacle)
                {
                    if (GameBoard->isSquareOccupied(Pos))
                    {
                        foundObstacle = true; // First obstacle found, next occupied square can be a capture target
                        continue;
                    }
                    else
                    {
                        PossibleMoves.Add(Pos); // Add as a potential move if no obstacle yet
                    }
                }
                else
                {
                    // After finding the first obstacle, look for the next occupied square to capture
                    if (GameBoard->isSquareOccupied(Pos))
                    {
                        PossibleMoves.Add(Pos); // Add this position as a special move
                        specialPossibleMove.Add(Pos);
                        break; // Stop searching in this direction after finding a capture target
                    }
                }
            }
        }
    }

    return PossibleMoves;
}

void APieceCannon::bePlacedSpecialSquareEffect_Implementation(AEnvSquare* squareDestination)
{
    moveMode = EPieceMoveMode::EParabolicJump;
}