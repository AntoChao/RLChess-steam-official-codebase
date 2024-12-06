// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

#include "../RLActor.h"
#include "../RLProduct.h"
#include "../../CommonEnum.h"
#include "Piece.generated.h"

UCLASS(BlueprintType, Blueprintable)
class APiece : public AActor, public IRLActor, public IRLProduct
{
	GENERATED_BODY()

public:
	APiece();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

/* RLActor functions*/
public:
	virtual FString GetActorName() override;

	virtual FString GetDescription() override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	virtual void BeInteracted(APlayerCharacter* Sender) override;

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;

	virtual int GetProductCost() override;

	virtual FString GetProductName() override;

	virtual UTexture2D* GetProductImage() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	virtual void inShopInteractedEffect(APlayerCharacter* Sender);

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	virtual void inBenchInteractedEffect(APlayerCharacter* Sender);

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	virtual void inBenchSpecialEffect();

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	virtual void inBoardInteractedEffect(APlayerCharacter* Sender);

/* Piece material information*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	UMaterialInterface* silverMaterial; // gray is neutrol
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	UMaterialInterface* redMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	UMaterialInterface* blueMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	UMaterialInterface* greenMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	UMaterialInterface* yellowMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	UMaterialInterface* purpleMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Piece Stats")
	TMap<FColor, UMaterialInterface*> colorToMaterial;

	UFUNCTION(BlueprintCallable, Category = "Piece Stats")
	void initializeMaterials();

/* Piece information*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString pieceName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString pieceDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FColor pieceColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	int pieceLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	EPieceStatus pieceStatus = EPieceStatus::EInShop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	EPieceDirection pieceDirection = EPieceDirection::ENone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	AEnvSquare* curSquare = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Piece Stats")
	void initializeDirection(AEnvSquare* squareDestination);

	UFUNCTION(BlueprintCallable, Category = "Piece Interaction")
	virtual TArray<FVector2D> calculatePossibleMove();


	// common calculatepossiblemove move function
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	FVector2D getDirectionVector(EPieceDirection Direction) const;

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	EPieceDirection directionVectorToEnum(FVector2D DirectionVector) const;

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	TArray<FVector2D> getDiagonalDirections() const;

	// all line by direction regarless obstacle
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	TArray<FVector2D> getLineMove(FVector2D CurrentLocation, EPieceDirection Direction, int Distance);

	// all line by direction including a obstacle or end of board
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	TArray<FVector2D> getLineMoveWithFirstObstacle(FVector2D CurrentLocation, EPieceDirection Direction, int Distance);

	// all line by direction before a obstacle or end of board
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	TArray<FVector2D> getLineMoveWithNoObstacle(FVector2D CurrentLocation, EPieceDirection Direction, int Distance);


	UFUNCTION(BlueprintCallable, Category = "Piece Interaction")
	virtual void dieEffect();


/* piece collision*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Piece Collision")
	UStaticMeshComponent* pieceBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Piece Collision")
	UBoxComponent* pieceCollision;


	UFUNCTION(BlueprintCallable, Category = "Piece Collision")
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

/* piece movement */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Movement")
	int movePoint = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Movement")
	bool isMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Stats")
	bool bHasMoved = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	EPieceStatus getPieceStatus();
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	void setPieceStatus(EPieceStatus newStatus);
	
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	void setPieceColor(FColor aColor);
	
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	FColor getPieceColor();

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	int getLevel();


	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	void bePlaced(AEnvSquare* squareDestination);

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	virtual void bePlacedInShopEffect(AEnvSquare* squareDestination);

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	virtual void bePlacedInBenchEffect(AEnvSquare* squareDestination);

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	void swapLocation(AEnvSquare* squareDestination);


	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	virtual void bePlacedInBoardEffect(AEnvSquare* squareDestination);

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	virtual void firstInBoardMovedEffect(AEnvSquare* squareDestination);

	/* all specific movement*/
	// just override start moving and end moving to simulate the type
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	virtual void startMoving(AEnvSquare* squareDestination);

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	virtual void endMoving(AEnvSquare* squareDestination);

};

