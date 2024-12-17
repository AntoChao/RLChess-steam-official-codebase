#include "PieceCatapult.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Environment/EnvBoard.h"

APieceCatapult::APieceCatapult()
{
}

TArray<FVector2D> APieceCatapult::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the catapult is on the board
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

        TArray<FVector2D> AdjacentDirections = {
            FVector2D(-1, 0), FVector2D(1, 0),  // Up, Down
            FVector2D(0, -1), FVector2D(0, 1)  // Left, Right
        };

        for (const FVector2D& Direction : AdjacentDirections)
        {
            FVector2D PotentialLocation = CurrentLocation + Direction;

            // Check if the potential location is valid and occupied
            if (GameBoard->isValidLocation(PotentialLocation) && GameBoard->isSquareOccupied(PotentialLocation))
            {
                EPieceDirection aDirection = directionVectorToEnum(Direction);
                TArray<FVector2D> allLaunchablePlaces = getLineMove(CurrentLocation, aDirection, INT_MAX);

                PossibleMoves.Append(allLaunchablePlaces);
                // PossibleMoves.Add(PotentialLocation);
            }
        }
    }

    return PossibleMoves;
}

void APieceCatapult::bePlacedInBoardEffect_Implementation(AEnvSquare* squareDestination)
{
    if (squareDestination)
    {
        lastMoveDirection = calculateMovingDirection(squareDestination);
        FVector2D directionVector = getDirectionVector(lastMoveDirection);
        FVector2D pieceLocation = curSquare->getSquareLocation() + directionVector;

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
            APiece* targetPiece = gameBoard->getSquareAtLocation(pieceLocation)->getOccupiedPiece();
            if (targetPiece)
            {
                targetPiece->beLaunchedTo(squareDestination);
            }

            gameBoard->resetBoard();
        }
    }
}