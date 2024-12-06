#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceKnight.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceKnight : public APiece
{
    GENERATED_BODY()

public:
    APieceKnight();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;
};