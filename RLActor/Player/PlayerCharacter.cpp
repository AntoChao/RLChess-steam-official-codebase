// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerRLController.h"
#include "InputActionValue.h"

#include "Kismet/GameplayStatics.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RoundManager.h"

#include "../AI/AIRLController.h"

#include "../RLActor.h"
#include "../Item/Item.h"
#include "../Piece/Piece.h"
#include "../Environment/EnvBoard.h"
#include "../Environment/EnvSquare.h"
#include "../Environment/EnvShop.h"
#include "Blueprint/UserWidget.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create the first-person camera
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	camera->SetupAttachment(RootComponent);
	camera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // Position at eye level
	camera->bUsePawnControlRotation = true; // Rotate camera with controller
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	inventory.SetNum(inventorySize);
}

void APlayerCharacter::setControllerInfo(FString aPlayerName, FColor aPlayerColor)
{
	playerName = aPlayerName;
	playerColor = aPlayerColor;
}

void APlayerCharacter::startSetup()
{
	setUpTime = true;
}
void APlayerCharacter::endSetup()
{
	// all pieces in bench should be in board right now
	for (APiece* eachPiece : army)
	{
		eachPiece->setPieceStatus(EPieceStatus::EInBoard);
	}

	setUpTime = false;
}

int APlayerCharacter::getPlayerSpeed() const
{
	return playerSpeed;
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

	// call round manager to end its turn if it is the current player playing
	/*
	if (isPlayerTurn)
	{
		URoundManager* roundManager = URoundManager::get();
		if (roundManager)
		{
			roundManager->endCurPlayerTurn();
		}
	}*/
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
void APlayerCharacter::setPlayerBench(TArray<AEnvSquare*> allSquares)
{
	playerBench = allSquares;
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
void APlayerCharacter::benchAPiece(APiece* newPiece)
{
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
bool APlayerCharacter::isEnableToBuyProduct(TScriptInterface<IRLProduct> aProduct)
{
	if (IRLProduct* specificProduct = aProduct.GetInterface())
	{
		int productCost = specificProduct->GetProductCost();

		bool hasSufficientMoney = (initMoney - productCost) >= 0;
		return isAbleToBenchPiece() && hasSufficientMoney;
	}
	return false;
}
void APlayerCharacter::payProduct(TScriptInterface<IRLProduct> aProduct)
{
	IRLProduct* specificProduct = aProduct.GetInterface();
	int productCost = specificProduct->GetProductCost();

	initMoney -= productCost;
}
void APlayerCharacter::receiveProduct(TScriptInterface<IRLProduct> aProduct)
{
	UObject* ProductObject = aProduct.GetObject(); // Get the underlying object
	if (APiece* Piece = Cast<APiece>(ProductObject))
	{
		Piece->setPieceColor(playerColor);
		benchAPiece(Piece);
	}
	else if (AItem* Item = Cast<AItem>(ProductObject))
	{
		return;
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

	/*
	// need a real time update of board status on the piece selection
	if ((isPlayerTurn) && IsValid(selectedPiece))
	{
		selectedPiece->BeInteracted(this);
	}*/
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

				if (DetectedInterface->IsAbleToBeInteracted(this))
				{
					// Interaction logic
					if (IsValid(inventory[selectedItemIndex]) &&
						inventory[selectedItemIndex]->isAbleToBeUsed(this, scriptInterface))
					{
						// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("DETECTION: USE ITEM"));
						curInteractionMode = EInteraction::EUseItem;
					}
					else if (hitActor->IsA(AItem::StaticClass()) && isAbleToPickUpItem())
					{
						// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("DETECTION: PICK UP ITEM"));
						curInteractionMode = EInteraction::EPickUpItem;
					}
					else if ((setUpTime || isPlayerTurn) && hitActor->IsA(APiece::StaticClass()))
					{
						// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("DETECTION: SELECT PIECE"));
						curInteractionMode = EInteraction::ESelectPiece;
					}
					else if ((setUpTime || isPlayerTurn) && IsValid(selectedPiece) && hitActor->IsA(AEnvSquare::StaticClass()))
					{
						// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("DETECTION: PLACE PIECE"));
						curInteractionMode = EInteraction::EPlacePiece;
					}
					else
					{
						// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("DETECTION: PLACE PIECE"));
						curInteractionMode = EInteraction::ENone;
					}
				}
				else
				{	
					curInteractionMode = EInteraction::ENone;
				}
            }
		}
	}
}

// turn control
void APlayerCharacter::startTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("LOGG: PLAYER STARTING TURN"));
	isPlayerTurn = true;
	selectedSquare = nullptr;
	unselectPiece();

	for (APiece* eachPiece : army)
	{
		eachPiece->updateStatus();
	}
}

void APlayerCharacter::endTurn()
{
	isPlayerTurn = false;
	moveSelectedPiece();
	unselectPiece();
}

// RL actor functions
FString APlayerCharacter::getPlayerName()
{
	return playerName;
}

FColor APlayerCharacter::getPlayerColor()
{
	return playerColor;
}

FString APlayerCharacter::GetActorName()
{
	return characterClassName;
}

FString APlayerCharacter::GetDescription()
{
	return characterDescription;
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
void APlayerCharacter::run() 
{
	if (isAbleToRun) {
		isRunning = true;
		curSpeed = runSpeed;
		updateSpeed();
	}
}

void APlayerCharacter::stopRun() 
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

void APlayerCharacter::selectItem(int itemIndex)
{
	selectedItemIndex = itemIndex;
}

void APlayerCharacter::interact()
{
	if (isAbleToInteract)
	{
		switch (curInteractionMode)
		{
		case EInteraction::EUseItem:
		{
			useItem();
			break;
		}
		case EInteraction::EPickUpItem:
		{
			pickUpItem();
			break;
		}
		case EInteraction::ESelectPiece:
		{
			selectPiece();
			break;
		}
		case EInteraction::EPlacePiece:
		{
			selectPlacePieceLocation();
			break;
		}
		case EInteraction::ENone:
		{
			unselectPiece();
			break;
		}
		default:
			break;
		}
	}
}

void APlayerCharacter::useItem()
{
	// choose and use item
	AItem* selectedItem = inventory[selectedItemIndex];

	selectedItem->beUsed(this, detectedActor);
	
	// delete item
	currentItemCount--;
	inventory[selectedItemIndex] = nullptr;
	selectedItem->Destroy();
}
void APlayerCharacter::pickUpItem()
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
void APlayerCharacter::selectPiece()
{
	unselectPiece();
	if (APiece* detectedPiece = Cast<APiece>(detectedActor.GetObject()))
	{
		selectedPiece = detectedPiece;
		detectedPiece->BeInteracted(this);
	}
}

void APlayerCharacter::selectPlacePieceLocation()
{
	// piece enter be place into new square
	selectedSquare = detectedActor;
	if (setUpTime)
	{
		moveSelectedPiece();
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Selecting place piece location"));
}

void APlayerCharacter::moveSelectedPiece()
{
	if (selectedSquare)
	{
		// piece enter be place into new square
 		selectedSquare->BeInteracted(this);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Placing Piece"));
	}
	unselectPiece();
}

bool APlayerCharacter::isAbleToPickUpItem()
{
	return currentItemCount >= 0 && currentItemCount <= inventorySize;
}

void APlayerCharacter::unselectPiece()
{
	selectedPiece = nullptr;

	AEnvBoard* gameBoard = UMapManager::get()->getGameBoard();
	if (gameBoard)
	{
		gameBoard->resetBoard();
	}
}

APiece* APlayerCharacter::getSelectedPiece()
{
	return selectedPiece;
}
void APlayerCharacter::setSelectedPiece(APiece* aPiece)
{
	selectedPiece = aPiece;
}

void APlayerCharacter::setSelectedSquare(AEnvSquare* aSquare)
{
	IRLActor* squareInterface = Cast<IRLActor>(aSquare);
	if (squareInterface)
	{
		TScriptInterface<IRLActor> scriptInterface;
		scriptInterface.SetObject(aSquare);
		scriptInterface.SetInterface(squareInterface);
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
	AEnvShop* gameShop = UMapManager::get()->getShop();

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