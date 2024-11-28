#include "PiecePawn.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APiecePawn::APiecePawn()
{
}

TArray<FVector2D> APiecePawn::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the pawn is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* GameBoard = Cast<AGameplayGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->getBoard();
    if (GameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();

        // Get the forward direction
        FVector2D ForwardDirection = GetForwardDirection();

        // Forward movement
        FVector2D OneStepForward = CurrentLocation + ForwardDirection;
        if (!GameBoard->isSquareOccupied(OneStepForward))
        {
            PossibleMoves.Add(OneStepForward);

            // Two-step forward if the pawn hasn't moved
            if (!bHasMoved)
            {
                FVector2D TwoStepForward = CurrentLocation + ForwardDirection * 2;
                if (GameBoard->isSquareOccupied(OneStepForward) && GameBoard->isSquareOccupied(TwoStepForward))
                {
                    PossibleMoves.Add(TwoStepForward);
                }
            }
        }

        // Diagonal capturing
        TArray<FVector2D> DiagonalDirections = GetDiagonalDirections();
        for (const FVector2D& DiagonalDirection : DiagonalDirections)
        {
            FVector2D DiagonalSquare = CurrentLocation + DiagonalDirection;
            if (GameBoard->isSquareOccupied(DiagonalSquare))
            {
                PossibleMoves.Add(DiagonalSquare);
            }
        }
    }
    
    return PossibleMoves;
}

FVector2D APiecePawn::GetForwardDirection() const
{
    switch (pieceDirection)
    {
    case EPieceDirection::EUp:    return FVector2D(0, 1);
    case EPieceDirection::EDown:  return FVector2D(0, -1);
    case EPieceDirection::ELeft:  return FVector2D(-1, 0);
    case EPieceDirection::ERight: return FVector2D(1, 0);
    default:                      return FVector2D(0, 0); // Default case
    }
}

TArray<FVector2D> APiecePawn::GetDiagonalDirections() const
{
    TArray<FVector2D> DiagonalDirections;

    switch (pieceDirection)
    {
    case EPieceDirection::EUp:
        DiagonalDirections.Add(FVector2D(-1, 1)); // Top-left
        DiagonalDirections.Add(FVector2D(1, 1));  // Top-right
        break;
    case EPieceDirection::EDown:
        DiagonalDirections.Add(FVector2D(-1, -1)); // Bottom-left
        DiagonalDirections.Add(FVector2D(1, -1));  // Bottom-right
        break;
    case EPieceDirection::ELeft:
        DiagonalDirections.Add(FVector2D(-1, -1)); // Left-bottom
        DiagonalDirections.Add(FVector2D(-1, 1));  // Left-top
        break;
    case EPieceDirection::ERight:
        DiagonalDirections.Add(FVector2D(1, -1));  // Right-bottom
        DiagonalDirections.Add(FVector2D(1, 1));   // Right-top
        break;
    default:
        break;
    }

    return DiagonalDirections;
}