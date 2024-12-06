#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceRanger.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceRanger : public APiece
{
    GENERATED_BODY()

public:
    APieceRanger();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

};