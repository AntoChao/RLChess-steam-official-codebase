#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceCannon.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceCannon : public APiece
{
    GENERATED_BODY()

public:
    APieceCannon();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

    virtual void bePlacedSpecialSquareEffect(AEnvSquare* squareDestination) override;
};