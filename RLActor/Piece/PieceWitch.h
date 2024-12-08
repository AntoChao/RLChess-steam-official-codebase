#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceWitch.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceWitch : public APiece
{
    GENERATED_BODY()

public:
    APieceWitch();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

    virtual void startMoving(AEnvSquare* squareDestination) override;

};