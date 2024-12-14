#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceSamurai.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceSamurai : public APiece
{
    GENERATED_BODY()

public:
    APieceSamurai();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

    virtual void killEffect_Implementation() override;
};