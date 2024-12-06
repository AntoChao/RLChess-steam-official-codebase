#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceWarrior.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceWarrior : public APiece
{
    GENERATED_BODY()

public:
    APieceWarrior();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

};