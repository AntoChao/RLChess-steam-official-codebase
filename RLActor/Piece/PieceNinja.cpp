#include "PieceNinja.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Environment/EnvBoard.h"

APieceNinja::APieceNinja()
{
}

TArray<FVector2D> APieceNinja::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the ninja is on the board
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
        TArray<EPieceDirection> Directions = {
            EPieceDirection::EUp, EPieceDirection::EDown,
            EPieceDirection::ELeft, EPieceDirection::ERight
        };

        for (const EPieceDirection& Direction : Directions)
        {
            FVector2D DirectionVector = getDirectionVector(Direction);

            // Step 1: Check for obstacles within a range of 1
            FVector2D AdjacentLocation = CurrentLocation + DirectionVector;
            if (gameBoard->isValidLocation(AdjacentLocation) && gameBoard->isSquareOccupied(AdjacentLocation))
            {
                PossibleMoves.Add(AdjacentLocation); // Can "kill" this piece
                continue; // Skip moving farther in this direction
            }

            // Step 2: Move as far as possible without obstacles
            TArray<FVector2D> LineMoves = getLineMoveWithNoObstacle(CurrentLocation, Direction, INT_MAX);
            PossibleMoves.Append(LineMoves);
        }
    }

    return PossibleMoves;
}
