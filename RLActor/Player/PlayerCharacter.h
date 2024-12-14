// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../RLActor.h"
#include "../../CommonEnum.h"

#include "../RLProduct.h"

#include "PlayerCharacter.generated.h"

// forward declaration
class USceneComponent;
class USkeletalMeshComponent;
class UCameraComponent;
struct FInputActionValue;

class UAnimMontage;
class USoundBase;

class AAIRLController;

class APiece;
class AItem;
class AEnvShop;

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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

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
	int playerSpeed;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	bool setUpTime = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	bool isAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float walkSpeed = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float runSpeed = 1000.0f;

	/* detection*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Stats")
	float rangeUnitDistance = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int rangeRank = 1;

	/* shopping*/
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int totalMoney = 5;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int curMoney = 5;

	/* inventory*/
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int inventorySize = 3;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int currentItemCount = 0;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	TArray<AItem*> inventory;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	TArray<AEnvSquare*> playerBench;

	/* army*/
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	TArray<APiece*> army;

public:
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

	/* inventory functions*/
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	int getInventorySize();

	/* bench functions*/
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void setPlayerBench(TArray<AEnvSquare*> allSquares);
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	bool isAbleToBenchPiece();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Character Stats")
	void benchAPiece(APiece* newPiece);

	/*shop functions*/
	UFUNCTION(BlueprintCallable, Category = "Shopoing Stats")
	bool isEnableToBuyProduct(APiece* aProduct);
	UFUNCTION(BlueprintCallable, Category = "Shopoing Stats")
	void payProduct(APiece* aProduct);
	UFUNCTION(Server, Reliable, Category = "Shopoing Stats")
	void receiveProduct(APiece* aProduct);

	/* army functions*/
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void pieceDied(APiece* pieceDie);

protected:
	virtual void Tick(float DeltaTime) override;

	/* detection*/
protected:

	UFUNCTION(BlueprintCallable, Category = "Detection Control")
	void detect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line Trace")
	bool fireDrawLine = true;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection Control")
	TScriptInterface<IRLActor> detectedActor;

	/* turn base stats*/
protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Components")
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

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interaction Controller")
	void selectItem(int itemIndex);

	/* controller interaction functions*/
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void interact();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void useItem();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void pickUpItem();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void selectPiece();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void selectPlacePieceLocation();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void moveSelectedPiece();

	/* controller helper functions*/
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	bool isAbleToPickUpItem();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
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
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	bool isAbleToMove = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float move_XSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float move_YSensitivity = 1.0f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	bool isRunning = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	bool isAbleToRun = true;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float curSpeed = 0.0f;

	// interaction stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	EInteraction curInteractionMode = EInteraction::ENone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	bool isAbleToInteract = true;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	APiece* selectedPiece = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	int selectedItemIndex = 0;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	TScriptInterface<IRLActor> selectedSquare = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	APiece* getSelectedPiece();
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void setSelectedPiece(APiece* aPiece);

	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void setSelectedSquare(AEnvSquare* aSquare);

	/* item Effect*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Effect")
	bool playerCanBeInteractived = true;

public:
	UFUNCTION(BlueprintCallable, Category = "Item Effect")
	void desableItemsUsage(float duration); // run a counter

/* AI relatived*/
protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "AI Controller")
	bool isAIPossessed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Controller")
	AAIRLController* myAIController = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category = "AI Controller")
	void setIsPossessedByAI(bool status, AAIRLController* aAIController);

	UFUNCTION(BlueprintCallable, Category = "AI Controller")
	void initializeArmy();

	UFUNCTION(BlueprintCallable, Category = "AI Controller")
	TArray<APiece*> getArmy();

	UFUNCTION(BlueprintCallable, Category = "AI Controller")
	void setRandomBench();

	UFUNCTION(BlueprintCallable, Category = "AI Controller")
	AEnvSquare* selectRandomBenchSquare();

	UFUNCTION(BlueprintCallable, Category = "AI Controller")
	void aiSelectMovement();
};

