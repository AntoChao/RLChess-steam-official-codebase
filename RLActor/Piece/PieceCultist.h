#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceCultist.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceCultist : public APiece
{
    GENERATED_BODY()

public:
    APieceCultist();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

    virtual void dieEffect(APiece* killer) override;
};