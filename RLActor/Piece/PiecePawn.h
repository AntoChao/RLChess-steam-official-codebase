#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "PiecePawn.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APiecePawn : public APiece
{
    GENERATED_BODY()

public:
    APiecePawn();

public:
    // Override the function to calculate possible moves for the pawn
    virtual TArray<FVector2D> calculatePossibleMove() override;
};