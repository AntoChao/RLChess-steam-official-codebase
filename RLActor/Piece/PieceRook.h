#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceRook.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceRook : public APiece
{
    GENERATED_BODY()

public:
    APieceRook();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;
};