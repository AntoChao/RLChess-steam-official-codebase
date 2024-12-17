#include "PieceDoctor.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Environment/EnvBoard.h"

APieceDoctor::APieceDoctor()
{
}

TArray<FVector2D> APieceDoctor::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the pawn is on the board
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

        // Get the forward direction based on the pawn's current facing
        FVector2D ForwardDirection = getDirectionVector(pieceDirection);

        // Forward movement up to movePoint spaces, handling initial move scenarios
        TArray<FVector2D> ForwardMoves = getLineMoveWithFirstObstacle(CurrentLocation, pieceDirection, movePoint);
        PossibleMoves.Append(ForwardMoves);
    }

    return PossibleMoves;
}