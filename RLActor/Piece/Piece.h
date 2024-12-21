// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TimerManager.h"
#include "Components/TimelineComponent.h"

#include "../RLActor.h"
#include "../RLProduct.h"
#include "../../CommonEnum.h"
#include "Piece.generated.h"

class UBoxComponent;
class UCurveFloat;
class ARLGameState;

class UStaticMeshComponent;
class APiecePreviewMesh;
class APieceFractureMesh;
class APieceConfirmedMesh;

UCLASS(BlueprintType, Blueprintable)
class APiece : public AActor, public IRLActor, public IRLProduct
{
	GENERATED_BODY()

public:
	APiece();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionOne(); // piece set piece color
	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionTwo(); // piece be place
	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionThree(); // piece be placed in shop
	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionFour(); // piece be placed in bench
	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionFive(); // piece be placed in board
	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionSix(); // piece start moving
	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionSeven(); // piece end moving
	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionEight();
	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunctionNine(); 
	/* RLActor functions*/
public:
	virtual FString GetActorName() override;

	virtual FString GetDescription() override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	UFUNCTION(BlueprintCallable, Category = "Piece")
	virtual void BeInteracted(APlayerCharacter* Sender) override;

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;


	virtual int GetProductCost() override;

	virtual FString GetProductName() override;

	virtual UTexture2D* GetProductImage() override;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	virtual void inShopInteractedEffect(APlayerCharacter* Sender);
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Piece Movement")
	virtual void inBenchInteractedEffect(APlayerCharacter* Sender);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	virtual void inBenchSpecialEffect();
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Piece Movement")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	UMaterialInterface* selectedMaterial;

	/* Piece information*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString pieceName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString pieceDescription;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FColor pieceColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	int pieceLevel;


	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	EPieceStatus pieceStatus = EPieceStatus::EInShop;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	EPieceDirection pieceDirection = EPieceDirection::ENone;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	AEnvSquare* curSquare = nullptr;

	UFUNCTION(Server, Unreliable, Category = "Piece Stats")
	void initializeDirection(AEnvSquare* squareDestination);

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
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	EPieceDirection getOppositeDirection(EPieceDirection Direction);

/* piece collision*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Piece Collision")
	UStaticMeshComponent* pieceStaticBodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Collision")
	UBoxComponent* pieceCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Collision")
	TSubclassOf<APiecePreviewMesh> piecePreviewMeshClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Collision")
	TSubclassOf<APieceConfirmedMesh> pieceConfirmedMeshClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Collision")
	TSubclassOf<APieceFractureMesh> pieceFractureMeshClass;

	UFUNCTION(Client, Reliable, BlueprintCallable, BlueprintCallable, Category = "debugFunction")
	void spawnFractureMesh(FVector aDirection); // piece set piece color

	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = "Piece Collision")
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Collision")
		void collidedWithOtherPiece(APiece* collidedPiece);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Collision")
		int collisionPriority = 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Piece Collision")
		bool isDied = false;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Interaction")
		void die(APiece* killer);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Piece Interaction")
		void dieEffect(APiece* killer);
		
	virtual void dieEffect_Implementation(APiece* killer);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Piece Collision")
		bool isCollided = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Piece Collision")
		FVector collisionLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Collision")
		float collideImpulseStrength = 1000.0f; // Adjust this value based on desired effect

public:
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	AEnvSquare* getOccupiedSquare();

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	int getPiecePriority();

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	bool getIsMoving();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	void setIsCollidedBy(APiece* collidedPiece);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Interaction")
	void kill(APiece* pieceToKill);

/* piece movement */
protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Piece Movement")
	int movePoint = 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Piece Movement")
	bool requireResting = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Piece Movement")
	bool isResting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Movement")
	int requiredRestingTurn = 0;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Piece Movement")
	int curRestingCount = 0;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Piece Movement")
	bool isMoving = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Pawn Stats")
	bool bHasMoved = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Pawn Stats")
	bool isKilledAnyActorThisTurn = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Pawn Stats")
	bool isKillEffectActive = true;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	void killEffect();

	virtual void killEffect_Implementation();

public:
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	EPieceStatus getPieceStatus();
	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void setPieceStatus(EPieceStatus newStatus);
	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void setPieceStatusInBoard();

	UFUNCTION(NetMulticast, Reliable, Category = "Piece Movement")
	void setPieceColor(FColor aColor);
	
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	FColor getPieceColor();

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	int getLevel();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	void updateStatusByTurn();
	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void updateRestStatus();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	void bePlaced(AEnvSquare* squareDestination);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Piece Movement")
	void setLocationMulti(FVector aLocation);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	virtual void bePlacedInShopEffect(AEnvSquare* squareDestination);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	void bePlacedInBenchEffect(AEnvSquare* squareDestination);

	virtual void bePlacedInBenchEffect_Implementation(AEnvSquare* squareDestination);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	void swapLocation(AEnvSquare* squareDestination);
	
	UFUNCTION(BlueprintCallable, Category = "Piece Interaction")
	virtual TArray<FVector2D> calculatePossibleMove();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	void bePlacedInBoardEffect(AEnvSquare* squareDestination);

	virtual void bePlacedInBoardEffect_Implementation(AEnvSquare* squareDestination);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	virtual void firstInBoardMovedEffect(AEnvSquare* squareDestination);

	TArray<FVector2D> specialPossibleMove;
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Interaction")
	void bePlacedSpecialSquareEffect(AEnvSquare* squareDestination);

	virtual void bePlacedSpecialSquareEffect_Implementation(AEnvSquare* squareDestination);

	/* all specific movement*/

	// just override start moving and end moving to simulate the type
	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void startMoving(AEnvSquare* squareDestination);

	virtual void startMoving_Implementation(AEnvSquare* squareDestination);

	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	EPieceDirection calculateMovingDirection(AEnvSquare* squareDestination);

	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void endMoving();

	virtual void endMoving_Implementation();

protected:
	/* general movement*/
	UPROPERTY(Replicated, EditAnywhere, Category = "Piece Movement")
	EPieceMoveMode moveMode = EPieceMoveMode::EGround;

	UPROPERTY(Replicated, EditAnywhere, Category = "Piece Movement")
	EPieceDirection lastMoveDirection = EPieceDirection::ENone;


	UPROPERTY(Replicated, VisibleAnywhere, Category = "Piece Movement")
	FTimeline movementTimeline; //951c0453780 error: 7FFA4CBE130D		
	UPROPERTY(EditAnywhere, Category = "Piece Movement")
	UCurveFloat* movementCurve;  // Set this in the Unreal Editor		

	UPROPERTY(Replicated, EditAnywhere, Category = "Piece Movement")
	FVector startLocation;
	UPROPERTY(Replicated, EditAnywhere, Category = "Piece Movement")
	FVector endLocation;
	UPROPERTY(Replicated, EditAnywhere, Category = "Piece Movement")
	AEnvSquare* targetSquare = nullptr;

	UPROPERTY(EditAnywhere, Category = "Piece Movement")
	float movementDuration = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Piece Movement")
	float jumpApexHeight = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Piece Movement")
	FTimerHandle TeleportTimerHandle;

	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void moveBasedOnMove(AEnvSquare* squareDestination);

	/* ground movement*/
	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void initiateGroundMovement(AEnvSquare* SquareDestination);
	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void handleGroundMovementProgress(float Value);

	/* parabolic jump*/
	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void initiateParabolicJump(AEnvSquare* SquareDestination);

	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void handleParabolicJumpProgress(float value);

	/* parabolic jump*/
	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void initiateKnightJump(AEnvSquare* SquareDestination);

	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void handleKnightJumpProgress(float value);

	/* teleport*/
	UFUNCTION(Server, Reliable, Category = "Piece Movement")
	void initializeTeleportation(AEnvSquare* SquareDestination);


	/* special interaction*/
protected:
	UPROPERTY(Replicated, EditAnywhere, Category = "Piece Movement")
	bool isLaunched = false;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	void launchEndEffect();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Piece Movement")
	void beExploted();

public:
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Piece Movement")
	void beLaunchedTo(AEnvSquare* SquareDestination);

	/* additional decoration*/
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	APiecePreviewMesh* getSpawnedPreviewMesh(FVector locationToSpawn);
	
	UFUNCTION(BlueprintCallable, Category = "Piece Movement")
	APieceConfirmedMesh* getSpawnedConfirmedMesh(FVector locationToSpawn);

};

