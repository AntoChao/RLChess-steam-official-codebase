#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceNinja.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceNinja : public APiece
{
    GENERATED_BODY()

public:
    APieceNinja();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

};