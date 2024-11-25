// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../RLActor.h"
#include "EnvSquare.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AEnvSquare : public AActor, public IRLActor
{
	GENERATED_BODY()

public:
	AEnvSquare();

public:
	virtual FString GetActorName() override;

	virtual FString GetDescription() override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	virtual void BeInteracted(APlayerCharacter* Sender) override;

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void beOccupied(APiece* aPiece);

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	FVector2D squareLocation = FVector2D(0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	bool isOccupied = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	APiece* occupiedPiece = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	FColor squareColorField;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	bool isPossibleMove = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void setSquareLocation(FVector2D aLocation);

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void setSquareColorField(FColor aColor);

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	FVector2D getSquareLocation();

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	FColor getSquareColorField();

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void setIsPossibleMove(bool status);

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void occupiedPieceLeaved();
};

