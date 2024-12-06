#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceDoggy.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceDoggy : public APiece
{
    GENERATED_BODY()

public:
    APieceDoggy();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

};