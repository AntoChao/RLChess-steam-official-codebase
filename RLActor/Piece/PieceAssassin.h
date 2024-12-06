#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceAssassin.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceAssassin : public APiece
{
    GENERATED_BODY()

public:
    APieceAssassin();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

};