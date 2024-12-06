#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PieceDoctor.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceDoctor : public APiece
{
    GENERATED_BODY()

public:
    APieceDoctor();

public:
    // Override the function to calculate possible moves for the rook
    virtual TArray<FVector2D> calculatePossibleMove() override;
};