#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "SimulatedBoardState.generated.h"

class APiece;

USTRUCT(BlueprintType)
struct FSimulatedPiece {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    APiece* Piece;
    UPROPERTY(BlueprintReadWrite)
    FVector2D PieceLocation;
    UPROPERTY(BlueprintReadWrite)
    bool bIsAIControlled;

    // Default constructor
    FSimulatedPiece()
        : Piece(nullptr), PieceLocation(FVector2D::ZeroVector), bIsAIControlled(false) {}

    // Custom constructor
    FSimulatedPiece(APiece* InPiece, const FVector2D& InLocation, bool bControlled)
        : Piece(InPiece), PieceLocation(InLocation), bIsAIControlled(bControlled) {}

    // Equality operator
    bool operator==(const FSimulatedPiece& Other) const {
        return Piece == Other.Piece &&
            PieceLocation == Other.PieceLocation &&
            bIsAIControlled == Other.bIsAIControlled;
    }
};

USTRUCT(BlueprintType)
struct FMove {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    APiece* Piece;           // The piece to be moved
    UPROPERTY(BlueprintReadWrite)
    FVector2D Target;        // The target location for the move

    // Constructor
    FMove()
        : Piece(nullptr), Target(FVector2D::ZeroVector) {}

    FMove(APiece* InPiece, const FVector2D& InTarget)
        : Piece(InPiece), Target(InTarget) {}
};

UCLASS(Blueprintable)
class USimulatedBoardState : public UObject
{
	GENERATED_BODY()

public:
    USimulatedBoardState();

public:
    UPROPERTY()
    TArray<FSimulatedPiece> PlayerPieces;

    UPROPERTY()
    TArray<FSimulatedPiece> OpponentPieces;

    void ApplyMove(FSimulatedPiece& SimPiece, const FVector2D& Target);
    void ResolveConflicts();
    bool IsGameOver() const;
    int32 EvaluateState() const;    
};