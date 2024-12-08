// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../RLActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInterface.h"
#include "EnvSquare.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AEnvSquare : public AActor, public IRLActor
{
	GENERATED_BODY()

public:
	AEnvSquare();

	virtual void BeginPlay() override;

public:
	virtual FString GetActorName() override;

	virtual FString GetDescription() override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	virtual void BeInteracted(APlayerCharacter* Sender) override;

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void beOccupied(APiece* aPiece);

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* squareMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* highlightMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* squareCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* placeScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Materials")
	UMaterialInterface* redMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Materials")
	UMaterialInterface* blueMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Materials")
	UMaterialInterface* greenMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Materials")
	UMaterialInterface* yellowMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Materials")
	UMaterialInterface* purpleMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Materials")
	UMaterialInterface* transparentMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Square Materials")
	TMap<FColor, UMaterialInterface*> colorToMaterial;

	UFUNCTION(BlueprintCallable, Category = "Square Materials")
	void initializeMaterials();

	UFUNCTION(BlueprintCallable, Category = "Square Materials")
	void setColor(const FColor& NewColor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	FVector2D squareLocation = FVector2D(0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	bool isOccupied = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	APiece* occupiedPiece = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	bool isPlayerOnTop = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	APlayerCharacter* playerOnTop = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	FColor squareColorField = FColor::White;

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
	void setIsPossibleMove(bool status, FColor pieceColor);

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void occupiedPieceLeaved();

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	bool getIsOccupied();
	
	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	APiece* getOccupiedPiece();

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	bool getIsPlayerOnTop();

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	APlayerCharacter* getPlayerOnTop();

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	FVector getPlacementLocation();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

