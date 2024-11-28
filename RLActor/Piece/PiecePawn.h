#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PiecePawn.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APiecePawn : public APiece
{
    GENERATED_BODY()

public:
    APiecePawn();

public:
    // Override the function to calculate possible moves for the pawn
    virtual TArray<FVector2D> calculatePossibleMove() override;

protected:
    // Helper function to get the forward direction vector based on the pawn's direction
    FVector2D GetForwardDirection() const;

    // Helper function to get diagonal directions for capturing
    TArray<FVector2D> GetDiagonalDirections() const;
};