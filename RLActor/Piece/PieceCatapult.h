#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceCatapult.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceCatapult : public APiece
{
    GENERATED_BODY()

public:
    APieceCatapult();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

    virtual void bePlacedInBoardEffect_Implementation(AEnvSquare* squareDestination) override;

protected:
    UPROPERTY(EditAnywhere, Category = "Piece Movement")
    APiece* launchPiece = nullptr;
};