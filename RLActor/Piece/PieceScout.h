#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceScout.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceScout : public APiece
{
    GENERATED_BODY()

public:
    APieceScout();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

protected:
    virtual void inBenchInteractedEffect(APlayerCharacter* Sender) override;

    virtual void bePlacedInBenchEffect_Implementation(AEnvSquare* squareDestination) override;
   
};