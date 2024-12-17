// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../RLActor.h"
#include "EnvSquare.h"
#include "EnvBoard.generated.h"

class UFactoryEnvironment;

UCLASS(BlueprintType, Blueprintable)
class AEnvBoard : public AActor, public IRLActor
{
	GENERATED_BODY()

public:
	AEnvBoard();

	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionOne(); // board initialize
	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionTwo(); // board initialize all color
	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionThree(); // board being reset

public:
	FString GetActorName() override;

	FString GetDescription() override;

	bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	void BeInteracted(APlayerCharacter* Sender) override;

	void BeUnInteracted(APlayerCharacter* Sender) override;

public:
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Board")
	void initialized();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Board")
	void initializeBoardColor(const TArray<APlayerCharacter*>& allPlayers);

	UFUNCTION(BlueprintCallable, Category = "Board")
	FVector getSpawnStartPositionForPlayer(int playerIndex);
	UFUNCTION(BlueprintCallable, Category = "Board")
	FRotator getSpawnStartRotationForPlayer(int playerIndex);

	UFUNCTION(BlueprintCallable, Category = "Board")
	FVector getPlayerPlacementOffset(int playerIndex);

	UFUNCTION(BlueprintCallable, Category = "Board")
	EPieceDirection calculateInitDirection(FVector2D initLocation);

protected:
	/* Board Stats*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
	int rowSize = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
	int columnSize = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
	int totalSquareNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
	FVector2D centerLocation = FVector2D(0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
	FVector boardCenter = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
	TArray<AEnvSquare*> allSquares;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
	float squareLength = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
	FRotator squareRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
	float pieceDistance = 100.0f;

protected:
	UFUNCTION(BlueprintCallable, Category = "Board")
	FVector2D getLocationFromIndex(int aIndex);
	UFUNCTION(BlueprintCallable, Category = "Board")
	int getIndexFromLocation(FVector2D aLocation);

public:
	UFUNCTION(BlueprintCallable, Category = "Board Stats")
	int getRowSize();
	UFUNCTION(BlueprintCallable, Category = "Board Stats")
	int getColumnSize();

	UFUNCTION(BlueprintCallable, Category = "Board Stats")
	int getSquareLength();
	
	UFUNCTION(BlueprintCallable, Category = "Board")
	void setSpecificColor(FColor aColor);

	UFUNCTION(BlueprintCallable, Category = "Board")
	void setAllUnoccupiedColor(FColor aColor);

	UFUNCTION(BlueprintCallable, Category = "Board")
	void setPossibleMoves(TArray<FVector2D> allPossibles, FColor pieceColor);

	UFUNCTION(BlueprintCallable, Category = "Board")
	void resetBoard();

	UFUNCTION(BlueprintCallable, Category = "Board")
	bool isSquareOccupied(FVector2D aLocation);
	UFUNCTION(BlueprintCallable, Category = "Board")
	bool isPlayerOnTop(FVector2D aLocation);

	UFUNCTION(BlueprintCallable, Category = "Board")
	bool isValidLocation(FVector2D aLocation);

	UFUNCTION(BlueprintCallable, Category = "Board")
	AEnvSquare* getSquareAtLocation(FVector2D aLocation);

	UFUNCTION(BlueprintCallable, Category = "Board")
	APiece* getPieceAtLocation(FVector2D aLocation);

	UFUNCTION(BlueprintCallable, Category = "Board")
	TArray<AEnvSquare*> getAllSquaresOfSpecificColor(FColor aColor);
};

