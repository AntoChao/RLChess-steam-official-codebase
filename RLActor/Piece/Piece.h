// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include "../RLActor.h"
#include "../../CommonEnum.h"
#include "Piece.generated.h"

UCLASS(BlueprintType, Blueprintable)
class APiece : public AActor, public IRLActor
{
	GENERATED_BODY()

public:
	APiece();

	virtual void BeginPlay() override;
/* RLActor functions*/
public:
	virtual FString GetActorName() override;

	virtual FString GetDescription() override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	virtual void BeInteracted(APlayerCharacter* Sender) override;

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;

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
	bool isMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Stats")
	bool bHasMoved = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	EPieceStatus getPieceStatus();
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	void setPieceStatus(EPieceStatus newStatus);

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	int getLevel();

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	void bePlaced(AEnvSquare* squareDestination);

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	virtual void firstMovedEffect(AEnvSquare* squareDestination);
};

