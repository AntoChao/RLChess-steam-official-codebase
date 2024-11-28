#include "PieceRook.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceRook::APieceRook()
{
}

TArray<FVector2D> APieceRook::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the rook is on the board
    {
        return PossibleMoves;
    }

    // Directions for rook movement: Up, Down, Left, Right
    TArray<FVector2D> Directions = {
        FVector2D(0, 1),   // Up
        FVector2D(0, -1),  // Down
        FVector2D(-1, 0),  // Left
        FVector2D(1, 0)    // Right
    };

    for (const FVector2D& Direction : Directions)
    {
        GenerateDirectionalMoves(Direction, PossibleMoves);
    }

    return PossibleMoves;
}

void APieceRook::GenerateDirectionalMoves(const FVector2D& Direction, TArray<FVector2D>& OutMoves) const
{
    if (!curSquare)
    {
        return;
    }

    AEnvBoard* GameBoard = Cast<AGameplayGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->getBoard();
    FVector2D CurrentLocation = curSquare->getSquareLocation();

    if (GameBoard)
    {
        // Maximum number of steps based on the board size
        int MaxSteps = FMath::Max(GameBoard->getRowSize(), GameBoard->getColumnSize());

        for (int Step = 1; Step < MaxSteps; ++Step)
        {
            CurrentLocation += Direction;

            // Add the move to the list
            OutMoves.Add(CurrentLocation);

            // Stop generating moves if the square is occupied
            if (GameBoard->isSquareOccupied(CurrentLocation))
            {
                break;
            }
        }
    }
}