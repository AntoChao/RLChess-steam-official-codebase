#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceGuard.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceGuard : public APiece
{
    GENERATED_BODY()

public:
    APieceGuard();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

};