#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceMage.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceMage : public APiece
{
    GENERATED_BODY()

public:
    APieceMage();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

    virtual void endMoving() override;
};