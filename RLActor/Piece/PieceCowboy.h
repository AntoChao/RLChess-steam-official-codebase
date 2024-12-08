#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceCowboy.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceCowboy : public APiece
{
    GENERATED_BODY()

public:
    APieceCowboy();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

    virtual void bePlacedInBoardEffect(AEnvSquare* squareDestination) override;

    virtual void bePlacedSpecialSquareEffect(AEnvSquare* squareDestination) override;
};