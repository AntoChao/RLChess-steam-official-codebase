#include "PieceQueen.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Environment/EnvBoard.h"

APieceQueen::APieceQueen()
{
}

TArray<FVector2D> APieceQueen::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the queen is on the board
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

        // Directions queen can move: all four diagonal directions plus the four cardinal directions
        TArray<EPieceDirection> Directions = {
            EPieceDirection::EUp, EPieceDirection::EDown,
            EPieceDirection::ELeft, EPieceDirection::ERight,
            EPieceDirection::EUpRight, EPieceDirection::EUpLeft,
            EPieceDirection::EDownRight, EPieceDirection::EDownLeft
        };

        // Iterate over each direction and calculate possible moves
        for (EPieceDirection Direction : Directions)
        {
            TArray<FVector2D> LineMoves = getLineMoveWithFirstObstacle(CurrentLocation, Direction, INT_MAX); // INT_MAX to simulate unlimited range
            PossibleMoves.Append(LineMoves);
        }
    }

    return PossibleMoves;
}