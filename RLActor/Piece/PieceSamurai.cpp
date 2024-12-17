#include "PieceSamurai.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Environment/EnvBoard.h"

APieceSamurai::APieceSamurai()
{
}

TArray<FVector2D> APieceSamurai::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the rook is on the board
    {
        return PossibleMoves;
    }

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
        FVector2D CurrentLocation = curSquare->getSquareLocation();

        // Directions rook can move: up, down, left, right
        TArray<EPieceDirection> Directions = {
            EPieceDirection::EUp, EPieceDirection::EDown,
            EPieceDirection::ELeft, EPieceDirection::ERight
        };

        // Iterate over each direction and calculate possible moves
        for (EPieceDirection Direction : Directions)
        {
            TArray<FVector2D> LineMoves = getLineMoveWithFirstObstacle(CurrentLocation, Direction, movePoint);            
            PossibleMoves.Append(LineMoves);
        }
    }

    return PossibleMoves;
}

void APieceSamurai::killEffect_Implementation()
{
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
        FVector2D CurrentLocation = curSquare->getSquareLocation();

        // Directions rook can move: up, down, left, right
        TArray<EPieceDirection> Directions = {
            EPieceDirection::EUp, EPieceDirection::EDown,
            EPieceDirection::ELeft, EPieceDirection::ERight
        };

        bool foundPlayer = false;

        // Iterate over each direction and calculate possible moves
        for (EPieceDirection Direction : Directions)
        {
            TArray<FVector2D> allMovesLocation = getLineMoveWithFirstObstacle(CurrentLocation, Direction, movePoint);
            
            // search for player to kill
            for (FVector2D moveLocation : allMovesLocation)
            {
                AEnvSquare* checkSquare = gameBoard->getSquareAtLocation(moveLocation);

                if (checkSquare && checkSquare->getIsPlayerOnTop())
                {
                    foundPlayer = true;
                    startMoving(checkSquare);
                    break;
                }
            }

            // search for normal piece to kill
            if (!foundPlayer)
            {
                for (FVector2D moveLocation : allMovesLocation)
                {
                    AEnvSquare* checkSquare = gameBoard->getSquareAtLocation(moveLocation);

                    if (checkSquare && checkSquare->getIsOccupied())
                    {
                        startMoving(checkSquare);
                        break;
                    }
                }
            }
        }
    }
}