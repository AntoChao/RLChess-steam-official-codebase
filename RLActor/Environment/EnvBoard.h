// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../RLActor.h"
#include "EnvSquare.h"
#include "EnvBoard.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AEnvBoard : public AActor, public IRLActor
{
	GENERATED_BODY()

public:
	AEnvBoard();

public:
	UFUNCTION(BlueprintCallable, Category = "Board")
	void createBoard();

	UFUNCTION(BlueprintCallable, Category = "Board")
	void initializeBoardColor(TArray<APlayerCharacter*> allPlayers);

	UFUNCTION(BlueprintCallable, Category = "Board")
	void setUpPlayerBench(TArray<APlayerCharacter*> allPlayers);

	UFUNCTION(BlueprintCallable, Category = "Board")
	FVector getSpawnStartPositionForPlayer(int playerIndex);

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
	FRotator squareRotation = FRotator(0.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
	float pieceDistance = 100.0f;

	UFUNCTION(BlueprintCallable, Category = "Board")
	FVector2D getLocationFromIndex(int aIndex);
	UFUNCTION(BlueprintCallable, Category = "Board")
	int getIndexFromLocation(FVector2D aLocation);

	UFUNCTION(BlueprintCallable, Category = "Board")
	bool isValidLocation(FVector2D aLocation);

public:
	FString GetActorName() override;

	FString GetDescription() override;

	bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	void BeInteracted(APlayerCharacter* Sender) override;

	void BeUnInteracted(APlayerCharacter* Sender) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Board Stats")
	int getRowSize();
	UFUNCTION(BlueprintCallable, Category = "Board Stats")
	int getColumnSize();

	UFUNCTION(BlueprintCallable, Category = "Board")
	void setSpecificColor(FColor aColor);

	UFUNCTION(BlueprintCallable, Category = "Board")
	void setPossibleMoves(TArray<FVector2D> allPossibles, FColor pieceColor);

	UFUNCTION(BlueprintCallable, Category = "Board")
	void resetBoard();

	UFUNCTION(BlueprintCallable, Category = "Board")
	bool isSquareOccupied(FVector2D aLocation);

};

