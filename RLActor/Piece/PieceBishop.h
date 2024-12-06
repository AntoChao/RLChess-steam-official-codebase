#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceBishop.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceBishop : public APiece
{
    GENERATED_BODY()

public:
    APieceBishop();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;
};