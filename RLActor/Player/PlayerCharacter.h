// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../RLActor.h"
#include "InputActionValue.h"
#include "../../CommonEnum.h"
#include "PlayerCharacter.generated.h"

// forward declaration
class USceneComponent;
class USkeletalMeshComponent;
class UCameraComponent;

class UAnimMontage;
class USoundBase;

class APiece;
class AItem;

/* Debug Tool
GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Control stop use weapon secondary"));
DrawDebugPoint(GetWorld(), TargetLocation, 10.0f, FColor::Red, false, 2.0f);
DrawDebugLine(GetWorld(), ViewStart, ViewEnd, FColor::Green, false, 1, 0, 1);

FString FloatString = FString::Printf(TEXT("MyFloatValue: %.2f"), MyFloatValue);
GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FloatString);
*/


UCLASS(BlueprintType, Blueprintable)
class APlayerCharacter : public ACharacter, public IRLActor
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay();

	/* character basic components*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float BodyRadius = 42.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float BodyHalfHeight = 96.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* camera;

	/* character basic stats*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	FString playerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	FColor playerColor;
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	FString characterClassName; // by default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	FString characterDescription; // by default


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	int playerSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	bool setUpTime = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	bool isAlive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float walkSpeed = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float runSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Stats")
	float rangeUnitDistance = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int rangeRank = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int initMoney = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int inventorySize = 3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int currentItemCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	TArray<AItem*> inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int benchSize = 10;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int currentBenchOccupation = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	TArray<APiece*> bench;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	FVector benchStartLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	FVector benchOffset;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int armyLevelCapacity = 10;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int curArmyLevel = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	TArray<APiece*> army;

public:
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void setControllerInfo(FString aPlayerName, FColor aPlayerColor);

	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	FString getPlayerName();
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	FColor getPlayerColor();

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void startSetup();
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void endSetup();

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	bool checkIsAlive();
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void setDied();
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void startDying();
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void beCollidedByPiece(APiece* pieceCollided);

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	int getInventorySize();

		/* bench functions*/
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void setBenchStartLocation(FVector aLocation);
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void setBenchOffset(FVector aOffset);

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	bool isAbleToBenchPiece();
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void benchAPiece(APiece* newPiece);

		/* army functions*/
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	bool isAbleToRecruit(APiece* piece);

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void recruitArmy(APiece* pieceToRecruit);

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void pieceDied(APiece* pieceDie);

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	int getArmySpeed();

protected:
	virtual void Tick(float DeltaTime) override;

	/* detection*/
protected:
	
	UFUNCTION(BlueprintCallable, Category = "Detection Control")
	void detect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection Control")
	FVector start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection Control")
	FVector forwardVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection Control")
	FVector end;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line Trace")
	FHitResult detectHit;

	FComponentQueryParams DefaultComponentQueryParams;
	FCollisionResponseParams DefaultResponseParam;

	UFUNCTION(BlueprintCallable, Category = "Detection Control")
	void detectReaction();
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection Control")
	IRLActor* detectedActor = nullptr; // has to be arlactor to ease
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection Control")
	IRLActor* inspectedActor = nullptr;

	/* turn base stats*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	bool isPlayerTurn = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Detection Control")
	void startTurn();

	UFUNCTION(BlueprintCallable, Category = "Detection Control")
	void endTurn();

	/* RL actor functions */
public:
	virtual FString GetActorName() override;

	virtual FString GetDescription() override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	virtual void BeInteracted(APlayerCharacter* Sender) override;

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;

	/* controller functions*/
public:
	UFUNCTION(BlueprintCallable, Category = "Control")
	void openMenu(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void look(FVector2D lookAxisVector);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void move(FVector2D movementVector);

	UFUNCTION(BlueprintCallable, Category = "Run Controller")
	void updateSpeed();
	UFUNCTION(BlueprintCallable, Category = "Run Controller")
	void run();
	UFUNCTION(BlueprintCallable, Category = "Run Controller")
	void stopRun();

	UFUNCTION(BlueprintCallable, Category = "jump Controller")
	void startJump();
	UFUNCTION(BlueprintCallable, Category = "jump Controller")
	void jumpCompleted();

	UFUNCTION(BlueprintCallable, Category = "Interaction Controller")
	void selectItem(int itemIndex);

	/* controller interaction functions*/
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void interact();
	
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void useItem();
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void pickUpItem();
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void shop();
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void selectPiece();
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void placePiece();

	/* controller helper functions*/
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	bool isAbleToPickUpItem();

	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void unselectPiece();

protected:
	// look stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look Controller Stats")
	bool isAbleToLook = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look Controller Stats")
	float baseTurnRate = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look Controller Stats")
	float baseLookUpRate = 45.0f;

	// move stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	bool isAbleToMove = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float move_XSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float move_YSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	bool isRunning = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	bool isAbleToRun = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float curSpeed = 0.0f;

	// interaction stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	EInteraction curInteractionMode = EInteraction::ENone;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	bool isAbleToInteract = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	APiece* selectedPiece = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	int selectedItemIndex = 0;

public:
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	APiece* getSelectedPiece();

	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void attachHUDToViewport(UUserWidget* shopHUD);

	/* item Effect*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Effect")
	bool playerCanBeInteractived = true;

public:
	UFUNCTION(BlueprintCallable, Category = "Item Effect")
	void desableItemsUsage(float duration); // run a counter 

};

