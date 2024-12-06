#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceQueen.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceQueen : public APiece
{
    GENERATED_BODY()

public:
    APieceQueen();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;

};