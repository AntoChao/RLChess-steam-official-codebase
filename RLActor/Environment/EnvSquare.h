// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../RLActor.h"
#include "Materials/MaterialInterface.h"
#include "EnvSquare.generated.h"

class APiece;
class APiecePreviewMesh;
class APieceConfirmedMesh;
class UStaticMeshComponent;
class UPrimitiveComponent;
class UBoxComponent;


UCLASS(BlueprintType, Blueprintable)
class AEnvSquare : public AActor, public IRLActor
{
	GENERATED_BODY()

public:
	AEnvSquare();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionOne(); //square BeInteracted
	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionTwo();

public:
	virtual FString GetActorName() override;

	virtual FString GetDescription() override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	virtual void BeInteracted(APlayerCharacter* Sender) override;

	UFUNCTION(Server, Reliable, Category = "Square Stats")
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

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Square Materials")
	void setColor(const FColor& NewColor);

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	FVector2D squareLocation = FVector2D(0.0f, 0.0f);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	bool isOccupied = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	APiece* occupiedPiece = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	APiecePreviewMesh* occupiedPreviewMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	APieceConfirmedMesh* occupiedConfirmedMesh = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	bool isPlayerOnTop = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	APlayerCharacter* playerOnTop = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	FColor squareColorField = FColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
	bool isPossibleMove = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void setSquareLocation(FVector2D aLocation);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Square Stats")
	void setSquareColorField(FColor aColor);

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	FVector2D getSquareLocation();
	
	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	FColor getSquareColorField();

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Square Stats")
	void setIsPossibleMove(bool status, FColor pieceColor);
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Square Stats")
	void setPreviewMesh(APiece* onePiece);
	// it should be call from netmulticast of board, but only after the prepare phrase
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Square Stats")
	void setConfirmedMesh(APiece* onePiece);

	UFUNCTION(Server, Reliable, Category = "Square Stats")
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

	UFUNCTION(Server, Reliable)
	void overlapEffect(APlayerCharacter* aPlayer);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable)
	void overlapEndEffect();
};

