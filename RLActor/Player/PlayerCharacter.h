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
class UHUDGameplay;

class UAnimMontage;
class USoundCue;

class AAIRLController;

class APiece;
class AItem;
class AEnvShop;
class APiecePreviewMesh;
class APieceFractureMesh;

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

protected:
	/* character color*/
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

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "Piece Stats")
	void initializeMaterials();

	UPROPERTY(ReplicatedUsing = OnRep_selectedMaterial, EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	UMaterialInterface* selectedMaterial;

	UFUNCTION(BlueprintCallable, Category = "Item Effect")
	void OnRep_selectedMaterial();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Main")
	UHUDGameplay* PlayerHUD;

	UFUNCTION(BlueprintCallable, Category = "Control")
	void updateWidget();

	/* character basic stats*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	int playerSpeed;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	bool setUpTime = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int totalMoney = 16;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int curMoney = 16;

	/* inventory*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int inventorySize = 3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int currentItemCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	TArray<AItem*> inventory;

	// the bench is replicated between each instance of the same player character in different world
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

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Character Stats")
	void startSetup();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Character Stats")
	void endSetup();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Character Stats")
	void spawnFracturePieces(APiece* pieceCollided);

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void beCollidedByPiece(APiece* pieceCollided);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character fracture")
	TSubclassOf<APieceFractureMesh> kingFractureMeshClass;

public:
	/* inventory functions*/
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	int getInventorySize();

	/* bench functions*/
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Character Stats")
	void setUpBench();
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	bool isAbleToBenchPiece();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Character Stats")
	void benchAPiece(APiece* newPiece);

	/*shop functions*/
	UFUNCTION(BlueprintCallable, Category = "Shopoing Stats")
	bool isEnableToBuyProduct(APiece* aProduct);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shopoing Stats")
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
	virtual FString GetActorName(ELanguage curLanguage) override;

	virtual FString GetDescription(ELanguage curLanguage) override;

	virtual FString GetInteractionDescription(ELanguage curLanguage) override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	virtual void BeInteracted(APlayerCharacter* Sender) override;

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Main")
	TSubclassOf<UUserWidget> menuHUDClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Main")
	UUserWidget* menuHUD;
	/* controller functions*/
public:
	UFUNCTION(BlueprintCallable, Category = "Control")
	void openMenu(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void look(FVector2D lookAxisVector);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void move(FVector2D movementVector);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Control")
	void moveServer(FVector2D movementVector);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Control")
	void moveMulticast(FVector2D movementVector);

	UFUNCTION(BlueprintCallable, Category = "Run Controller")
	void updateSpeed();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Run Controller")
	void run();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Run Controller")
	void runMulticast();

	UFUNCTION(Server, Unreliable, BlueprintCallable, Category = "Run Controller")
	void stopRun();

	UFUNCTION(BlueprintCallable, Category = "jump Controller")
	void startJump();
	UFUNCTION(BlueprintCallable, Category = "jump Controller")
	void jumpCompleted();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interaction Controller")
	void selectItem(int itemIndex);

	/* controller interaction functions*/
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void interact();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void useItem();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void pickUpItem();

	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void buyPiece(APiece* detectedPiece);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void serverBuyInShopPiece();
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void swapPieceLocations(APiece* detectedPiece);
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void selectPiece(APiece* detectedPiece);
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void attackPiece(APiece* pieceToAttack);

	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void selectPlacePieceLocationBySquare2(AEnvSquare* detectedSquare);
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void selectPlacePieceLocationByPreview(APiecePreviewMesh* detectedMesh);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void moveSelectedPiece();

	/* controller helper functions*/
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	bool isAbleToPickUpItem();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void unselectPiece();
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Interact Control")
	void clientResetBoard();

public:
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	APiece* getSelectedPiece();
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	APiece* getConfirmedPiece();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void setSelectedPiece(APiece* aPiece);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void setConfirmedPiece(APiece* aPiece);

	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void setSelectedSquare(AEnvSquare* aSquare);
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
	void setSelectedSquareEffect(AEnvSquare* aSquare);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interact Control")
	void setSelectedSquareValue(AEnvSquare* aSquare);

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

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	APiece* selectedPiece = nullptr;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	APiece* confirmedPiece = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	int selectedItemIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	AEnvSquare* selectedSquare = nullptr;

	/* item Effect*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Effect")
	bool playerCanBeInteractived = true;

public:
	UFUNCTION(BlueprintCallable, Category = "Item Effect")
	void desableItemsUsage(float duration); // run a counter

/* AI relatived*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Controller")
	bool isAIPossessed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Controller")
	AAIRLController* myAIController = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category = "AI Controller")
	void setIsPossessedByAI(bool status, AAIRLController* aAIController);

	UFUNCTION(BlueprintCallable, Category = "AI Controller")
	void initializeRandomArmy();

	UFUNCTION(BlueprintCallable, Category = "AI Controller")
	TArray<APiece*> getArmy();

	UFUNCTION(BlueprintCallable, Category = "AI Controller")
	void setRandomBench();

	UFUNCTION(BlueprintCallable, Category = "AI Controller")
	AEnvSquare* selectRandomBenchSquare();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundCue* selectPieceSC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundCue* unselectPieceSC;
};

