// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerRLController.h"
#include "InputActionValue.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../AI/AIRLController.h"

#include "../RLActor.h"
#include "../Item/Item.h"
#include "../Piece/Piece.h"
#include "../Piece/PiecePreviewMesh.h"

#include "../Environment/EnvBoard.h"
#include "../Environment/EnvSquare.h"
#include "../Environment/EnvShop.h"
#include "Blueprint/UserWidget.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	NetUpdateFrequency = 150.0f; // 100 updates per second
	MinNetUpdateFrequency = 50.0f;

	// Enable network replication
	bReplicates = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create the first-person camera
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	camera->SetupAttachment(RootComponent);
	camera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // Position at eye level
	camera->bUsePawnControlRotation = true; // Rotate camera with controller
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health.
	DOREPLIFETIME(APlayerCharacter, setUpTime);
	DOREPLIFETIME(APlayerCharacter, isPlayerTurn);
	DOREPLIFETIME(APlayerCharacter, playerBench);
	DOREPLIFETIME(APlayerCharacter, army);

	DOREPLIFETIME(APlayerCharacter, selectedPiece);
	DOREPLIFETIME(APlayerCharacter, selectedSquare);

	DOREPLIFETIME(APlayerCharacter, curMoney);

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	inventory.SetNum(inventorySize);
	curMoney = totalMoney;
}

void APlayerCharacter::startSetup_Implementation()
{
	setUpTime = true;
	debugFunctionOne();
}
void APlayerCharacter::endSetup_Implementation()
{
	// all pieces in bench should be in board right now
	for (APiece* eachPiece : army)
	{
		eachPiece->setPieceStatus(EPieceStatus::EInBoard);
	}

	setUpTime = false;
}

bool APlayerCharacter::checkIsAlive()
{
	return isAlive;
}

/*
player die if:
	got collided with a piece movement
	player has no piece in board after initial setup
*/
void APlayerCharacter::setDied()
{
	isAlive = false;

	AController* aController = GetController();
	if (Controller)
	{
		APlayerRLController* PlayerController = Cast<APlayerRLController>(aController);
		if (PlayerController)
		{
			PlayerController->UnPossessEffect();
		}
	}
}
void APlayerCharacter::startDying()
{
	return;
}
void APlayerCharacter::beCollidedByPiece(APiece* pieceCollided)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("player be collided"));

	startDying();
}

int APlayerCharacter::getInventorySize()
{
	return inventorySize;
}

/* bench function*/
void APlayerCharacter::setUpBench_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UWorld* serverWorld = GetWorld();
		if (serverWorld)
		{
			ARLGameState* serverGameState = Cast<ARLGameState>(serverWorld->GetGameState());
			if (serverGameState)
			{
				AEnvBoard* serverBoard = serverGameState->getGameBoard();

				if (serverBoard)
				{

					playerBench = serverBoard->getAllSquaresOfSpecificColor(getPlayerColor());
				}
			}
		}
	}
}
bool APlayerCharacter::isAbleToBenchPiece()
{
	for (AEnvSquare* aBenchSquare : playerBench)
	{
		if (!aBenchSquare->getIsOccupied())
		{
			return true;
		}
	}
	return false;
}
void APlayerCharacter::benchAPiece_Implementation(APiece* newPiece)
{
	debugFunctionThree();

	army.Add(newPiece);
	for (AEnvSquare* aBenchSquare : playerBench)
	{
		if (!aBenchSquare->getIsOccupied())
		{
			newPiece->bePlaced(aBenchSquare);
			break;
		}
	}
}

/* shop functions*/
bool APlayerCharacter::isEnableToBuyProduct(APiece* aProduct)
{
	if (aProduct)
	{
		int productCost = aProduct->GetProductCost();

		bool hasSufficientMoney = (curMoney - productCost) >= 0;
		return isAbleToBenchPiece() && hasSufficientMoney;
	}
	return false;
}
	// no server because others dont care the money 
void APlayerCharacter::payProduct_Implementation(APiece* aProduct)
{
	// IRLProduct* specificProduct = aProduct.GetInterface();
	int productCost = aProduct->GetProductCost();

	curMoney -= productCost;
}
void APlayerCharacter::receiveProduct_Implementation(APiece* aProduct)
{
	if (aProduct)
	{
		aProduct->setPieceColor(getPlayerColor());
		benchAPiece(aProduct);
	}
}

/* army function*/
void APlayerCharacter::pieceDied(APiece* pieceDie)
{
	// pieceDie->dieEffect();
	army.Remove(pieceDie);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	// Call the base class  
	Super::Tick(DeltaTime);

	detect();
}

void APlayerCharacter::detect()
{
	//Expose function, should not be an attack
	if (IsValid(camera)) {
		start = camera->GetComponentLocation();
		forwardVector = camera->GetForwardVector();
		end = ((forwardVector * rangeUnitDistance * rangeRank) + start);

		if (fireDrawLine)
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 1, 0, 1);
		}
		
		GetWorld()->LineTraceSingleByChannel(detectHit, start, end, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam);
		detectReaction();
	}
}

void APlayerCharacter::detectReaction()
{
	if (detectHit.bBlockingHit)
	{
		AActor* hitActor = detectHit.GetActor();
		UClass* detectedActorClass = hitActor->GetClass(); // Fixed `Actor` to `rlActor`

		if (hitActor && detectedActorClass->ImplementsInterface(URLActor::StaticClass()))
		{

			// Safely retrieve the interface
            IRLActor* DetectedInterface = Cast<IRLActor>(hitActor);
            if (DetectedInterface)
            {
                // Create TScriptInterface for interaction
                TScriptInterface<IRLActor> scriptInterface;
				scriptInterface.SetObject(hitActor); // Set the UObject part
				scriptInterface.SetInterface(DetectedInterface); // Set the interface pointer

				// Update inspected actor
				detectedActor = scriptInterface;

				// should only show the information
            }
		}
	}
}

// turn control
void APlayerCharacter::startTurn()
{
	debugFunctionSeven();

	UE_LOG(LogTemp, Warning, TEXT("LOGG: PLAYER STARTING TURN"));
	isPlayerTurn = true;
	selectedSquare = nullptr;
	unselectPiece();
	clientResetBoard();

	for (int i = 0; i < army.Num(); i++)
	{
		if (army[i])
		{
			APiece* eachPiece = army[i];

			if (eachPiece)
			{
				eachPiece->updateStatusByTurn();
			}
			else
			{
				// got eliminated
				army.Remove(eachPiece);
			}
		}
	}
}

void APlayerCharacter::endTurn()
{
	debugFunctionEight();

	isPlayerTurn = false;
	moveSelectedPiece();
	unselectPiece();
	clientResetBoard();
}

// RL actor functions
FString APlayerCharacter::getPlayerName()
{
	if (APlayerRLController* PC = Cast<APlayerRLController>(GetController()))
	{
		return PC->getPlayerName();
	}
	return TEXT("");
}

FColor APlayerCharacter::getPlayerColor()
{
	if (APlayerRLController* PC = Cast<APlayerRLController>(GetController()))
	{
		return PC->getPlayerColor();
	}
	return FColor::Purple;
}

FString APlayerCharacter::GetActorName()
{
	return getPlayerName();
}

FString APlayerCharacter::GetDescription()
{
	return TEXT("");
}

bool APlayerCharacter::IsAbleToBeInteracted(APlayerCharacter* Sender)
{
	return playerCanBeInteractived;
}

void APlayerCharacter::BeInteracted(APlayerCharacter* Sender)
{
	return;
}

void APlayerCharacter::BeUnInteracted(APlayerCharacter* Sender)
{
	return;
}

void APlayerCharacter::openMenu(const FInputActionValue& Value)
{
	return;
}

void APlayerCharacter::look(FVector2D lookAxisVector)
{
	if (isAbleToLook)
	{
		AddControllerYawInput(lookAxisVector.X * baseTurnRate * GetWorld()->GetDeltaSeconds());
		AddControllerPitchInput(-1 * lookAxisVector.Y * baseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void APlayerCharacter::move(FVector2D movementVector)
{
	if (isAbleToMove) {
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, movementVector.Y * move_XSensitivity);
		AddMovementInput(RightDirection, movementVector.X * move_YSensitivity);
	}
}

void APlayerCharacter::updateSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = curSpeed;
}
void APlayerCharacter::run_Implementation()
{
	if (isAbleToRun) {
		isRunning = true;
		curSpeed = runSpeed;
		updateSpeed();
	}
}

void APlayerCharacter::stopRun_Implementation()
{
	isRunning = false;
	curSpeed = walkSpeed;
	updateSpeed();
}

void APlayerCharacter::startJump()
{
	Jump();
}
void APlayerCharacter::jumpCompleted()
{
	StopJumping();
}

void APlayerCharacter::selectItem_Implementation(int itemIndex)
{
	selectedItemIndex = itemIndex;
}

void APlayerCharacter::interact()
{
	if (detectHit.bBlockingHit)
	{
		AActor* hitActor = detectHit.GetActor();
		UClass* detectedActorClass = hitActor->GetClass(); // Fixed `Actor` to `rlActor`

		if (hitActor && detectedActorClass->ImplementsInterface(URLActor::StaticClass()))
		{
			IRLActor* DetectedInterface = Cast<IRLActor>(hitActor);
			if (DetectedInterface)
			{
				TScriptInterface<IRLActor> scriptInterface;
				scriptInterface.SetObject(hitActor); // Set the UObject part
				scriptInterface.SetInterface(DetectedInterface); // Set the interface pointer

				detectedActor = scriptInterface;

				if (DetectedInterface->IsAbleToBeInteracted(this))
				{
					// Interaction logic
					if (IsValid(inventory[selectedItemIndex]) &&
						inventory[selectedItemIndex]->isAbleToBeUsed(this, scriptInterface))
					{
						useItem();
					}
					else if (hitActor->IsA(AItem::StaticClass()) && isAbleToPickUpItem())
					{
						pickUpItem();
					}
					else if ((setUpTime || isPlayerTurn) && hitActor->IsA(APiece::StaticClass()))
					{
						APiece* detectedPiece = Cast<APiece>(hitActor);
						if (detectedPiece->getPieceStatus() == EPieceStatus::EInShop || detectedPiece->getPieceColor() == getPlayerColor())
						{
							selectPiece(detectedPiece);
						}
						else
						{
							if (IsValid(selectedPiece))
							{
								attackPiece(detectedPiece);
							}
							else
							{
								unselectPiece();
								clientResetBoard();
							}
						}
					}
					else if ((setUpTime || isPlayerTurn) && IsValid(selectedPiece))
					{
						if (hitActor->IsA(AEnvSquare::StaticClass()))
						{
							AEnvSquare* squareToPlace = Cast<AEnvSquare>(hitActor);
							selectPlacePieceLocationBySquare(squareToPlace);
						}
						else if (hitActor->IsA(APiecePreviewMesh::StaticClass()))
						{
							APiecePreviewMesh* previewMeshDetected = Cast<APiecePreviewMesh>(hitActor);
							selectPlacePieceLocationByPreviewMesh(previewMeshDetected);
						}
						else
						{
							unselectPiece();
						}
					}
					else
					{
						unselectPiece();
					}
				}
				else
				{
					unselectPiece();
				}
			}
		}
	}
}

void APlayerCharacter::useItem_Implementation()
{
	// choose and use item
	AItem* selectedItem = inventory[selectedItemIndex];

	selectedItem->beUsed(this, detectedActor);
	
	// delete item
	currentItemCount--;
	inventory[selectedItemIndex] = nullptr;
	selectedItem->Destroy();
}
void APlayerCharacter::pickUpItem_Implementation()
{
	for (AItem* eachItem : inventory)
	{
		if (eachItem == nullptr)
		{
			detectedActor->BeInteracted(this);
			currentItemCount++;
			break;
		}
	}
}

/*
selectpiece -> 
queue: unselectpiece -> clientResetBoard -> setselected piece -> inshop/inbench/inboard
	   piece = null  -> reset board -> piece = selected -> effect

*/

void APlayerCharacter::selectPiece(APiece* detectedPiece) // non rpc
{
	unselectPiece(); // server
	clientResetBoard();
	if (detectedPiece)
	{
		// detectedPiece->BeInteracted(this);
		setSelectedPiece(detectedPiece); // server

		switch (detectedPiece->getPieceStatus())
		{
		case EPieceStatus::EInShop:
		{
			serverSelectInShopPiece(); //server
			break;
		}
		case EPieceStatus::EInBench:
		{
			detectedPiece->inBenchInteractedEffect(this); // client rpc
			break;
		}
		case EPieceStatus::EInBoard:
		{
			detectedPiece->inBoardInteractedEffect(this); // client rpc
			break;
		}
		default:
			break;
		}
	}
}

void APlayerCharacter::serverSelectInShopPiece_Implementation()
{
	if (selectedPiece)
	{
		selectedPiece->inShopInteractedEffect(this);
	}
}

void APlayerCharacter::attackPiece(APiece* pieceToAttack)
{
	if (pieceToAttack)
	{
		setSelectedSquare(pieceToAttack->getOccupiedSquare());

		clientResetBoard();
	}
}

void APlayerCharacter::selectPlacePieceLocationBySquare(AEnvSquare* detectedSquare)
{
	if (detectedSquare)
	{
		setSelectedSquare(detectedSquare);
		if (setUpTime)
		{
			debugFunctionOne();
			moveSelectedPiece();
		}
		clientResetBoard();
	}
}

void APlayerCharacter::selectPlacePieceLocationByPreviewMesh(APiecePreviewMesh* detectedMesh)
{
	if (detectedMesh)
	{
		AEnvSquare* detectedSquare = detectedMesh->getPreviewSquare();
		setSelectedSquare(detectedSquare);
		if (setUpTime)
		{
			debugFunctionOne();
			moveSelectedPiece();
		}
		clientResetBoard();
	}
}

void APlayerCharacter::moveSelectedPiece_Implementation()
{
	if (selectedSquare)
	{
		debugFunctionTwo();
		// piece enter be place into new square
 		selectedSquare->BeInteracted(this);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Placing Piece"));
	}
	unselectPiece();
	clientResetBoard();
}

bool APlayerCharacter::isAbleToPickUpItem()
{
	return currentItemCount >= 0 && currentItemCount <= inventorySize;
}

void APlayerCharacter::unselectPiece_Implementation() // Client
{
	selectedPiece = nullptr;
}

void APlayerCharacter::clientResetBoard_Implementation()
{
	AEnvBoard* gameBoard = nullptr;
	if (UWorld* World = GetWorld())
	{
		ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
		if (GameState)
		{
			gameBoard = GameState->getGameBoard();

			if (gameBoard)
			{
				gameBoard->resetBoard(); // non rpc
			}
		}
	}
}

APiece* APlayerCharacter::getSelectedPiece()
{
	return selectedPiece;
}
void APlayerCharacter::setSelectedPiece_Implementation(APiece* aPiece) // client
{
	selectedPiece = aPiece;
}

void APlayerCharacter::setSelectedSquare(AEnvSquare* aSquare) // non rpc
{
	setSelectedSquareValue(aSquare);
	setSelectedSquareEffect(aSquare);
	debugFunctionThree();
}

void APlayerCharacter::setSelectedSquareValue_Implementation(AEnvSquare* aSquare) // server
{
	if (setUpTime || isPlayerTurn)
	{
		if (aSquare && selectedPiece)
		{
			selectedSquare = aSquare;
		}
	}
}
void APlayerCharacter::setSelectedSquareEffect(AEnvSquare* aSquare) // non rpc
{
	if (selectedSquare)
	{
		selectedSquare->setConfirmedMesh(nullptr); // client
	}

	if (setUpTime || isPlayerTurn)
	{
		if (aSquare && selectedPiece)
		{
			if (isPlayerTurn)
			{
				aSquare->setConfirmedMesh(selectedPiece); // client
			}
		}
	}
}

/* item Effect*/
void APlayerCharacter::desableItemsUsage(float duration)
{
	return;
}

/* AI relatived*/
void APlayerCharacter::setIsPossessedByAI(bool status, AAIRLController* aAIController)
{
	isAIPossessed = status;
	myAIController = aAIController;
}

// call at beginning of setup
void APlayerCharacter::initializeArmy()
{
	AEnvShop* gameShop = nullptr;
	if (UWorld* World = GetWorld())
	{
		ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
		if (GameState)
		{
			gameShop = GameState->getShop();
		}
	}

	gameShop->fullFill(this);
}

TArray<APiece*> APlayerCharacter::getArmy()
{
	return army;
}

// call at end of setup
void APlayerCharacter::setRandomBench()
{
	for (APiece* eachPiece : army)
	{
		AEnvSquare* randomBenchSquare = selectRandomBenchSquare();

		eachPiece->bePlaced(randomBenchSquare);
	}
}

AEnvSquare* APlayerCharacter::selectRandomBenchSquare()
{
	int randomInt = FMath::RandRange(0, playerBench.Num() - 1);

	return playerBench[randomInt];
}

// called at the end of selecting phrase
void APlayerCharacter::aiSelectMovement()
{
	if (myAIController)
	{
		myAIController->setCurrentValueMap();
		myAIController->makeTheBestMove();
		moveSelectedPiece();
	}
}