// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerRLController.h"

#include "Kismet/GameplayStatics.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RoundManager.h"

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
	bench.SetNum(benchSize);
}

void APlayerCharacter::setControllerInfo(FString aPlayerName, FColor aPlayerColor)
{
	playerName = aPlayerName;
	playerColor = aPlayerColor;
}

void APlayerCharacter::startSetup()
{
	setUpTime = true;
	isPlayerTurn = true;
}
void APlayerCharacter::endSetup()
{
	setUpTime = false;
	isPlayerTurn = false;
}

bool APlayerCharacter::checkIsAlive()
{
	return isAlive;
}

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
	startDying();
}

int APlayerCharacter::getInventorySize()
{
	return inventorySize;
}

/* bench function*/
void APlayerCharacter::setBenchStartLocation(FVector aLocation)
{
	benchStartLocation = aLocation;
}
void APlayerCharacter::setBenchOffset(FVector aOffset)
{
	benchOffset = aOffset;
}

bool APlayerCharacter::isAbleToBenchPiece()
{
	return currentBenchOccupation <= benchSize;
}
void APlayerCharacter::benchAPiece(APiece* newPiece)
{
	for (int i = 0; i < benchSize; i++)
	{
		APiece* firstEmptyPiece = bench[i];
		if (firstEmptyPiece == nullptr)
		{
			firstEmptyPiece = newPiece;
			firstEmptyPiece->SetActorLocation(benchStartLocation + (benchOffset * i));
		}
	}
}

/* army function*/
bool APlayerCharacter::isAbleToRecruit(APiece* piece)
{
	return (curArmyLevel + piece->getLevel()) <= armyLevelCapacity;
}

void APlayerCharacter::recruitArmy(APiece* pieceToRecruit)
{
	curArmyLevel += pieceToRecruit->getLevel();
	army.Add(pieceToRecruit);
}

void APlayerCharacter::pieceDied(APiece* pieceDie)
{
	curArmyLevel -= pieceDie->getLevel();
	army.Remove(pieceDie);
}

int APlayerCharacter::getArmySpeed()
{
	return playerSpeed;
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

		DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 1, 0, 1);

		GetWorld()->LineTraceSingleByChannel(detectHit, start, end, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam);
		detectReaction();
	}
}

void APlayerCharacter::detectReaction()
{
	if (detectHit.bBlockingHit)
	{
		AActor* rlActor = detectHit.GetActor();
		UClass* detectedActorClass = rlActor->GetClass(); // Fixed `Actor` to `rlActor`

		if (rlActor && detectedActorClass->ImplementsInterface(URLActor::StaticClass()))
		{
			detectedActor = Cast<IRLActor>(rlActor);

			if (detectedActor->IsAbleToBeInteracted(this))
			{
				TScriptInterface<IRLActor> ScriptInterface;
				ScriptInterface.SetObject(Cast<UObject>(detectedActor));   // Set the UObject part
				ScriptInterface.SetInterface(detectedActor);

				if (IsValid(inventory[selectedItemIndex]) && inventory[selectedItemIndex]->isAbleToBeUsed(this, ScriptInterface))
				{
					curInteractionMode = EInteraction::EUseItem;
				}
				else if (detectedActorClass->IsChildOf(AItem::StaticClass()) && isAbleToPickUpItem())
				{
					curInteractionMode = EInteraction::EPickUpItem;
				}
				else if (detectedActorClass->IsChildOf(AEnvShop::StaticClass())) // Missing closing parenthesis
				{
					curInteractionMode = EInteraction::EShop;
				}
				else if (isPlayerTurn && detectedActorClass->IsChildOf(APiece::StaticClass()))
				{
					curInteractionMode = EInteraction::ESelectPiece;
				}
				else if (isPlayerTurn && IsValid(selectedPiece) && detectedActorClass->IsChildOf(AEnvSquare::StaticClass()))
				{
					curInteractionMode = EInteraction::EPlacePiece;
				}
				else
				{
					curInteractionMode = EInteraction::ENone;
				}
			}

			/* inspection for details */
			inspectedActor = detectedActor;
		}
		else
		{
			inspectedActor = nullptr;
		}
	}
}

// turn control
void APlayerCharacter::startTurn()
{
	isPlayerTurn = true;
}

void APlayerCharacter::endTurn()
{
	isPlayerTurn = false;
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

// controller
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
		case EInteraction::EShop:
		{
			shop();
			break;
		}
		case EInteraction::ESelectPiece:
		{
			selectPiece();
			break;
		}
		case EInteraction::EPlacePiece:
		{
			placePiece();
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

	TScriptInterface<IRLActor> ScriptInterface;
	ScriptInterface.SetObject(Cast<UObject>(detectedActor));   // Set the UObject part
	ScriptInterface.SetInterface(detectedActor);

	selectedItem->beUsed(this, ScriptInterface);
	
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
void APlayerCharacter::shop()
{
	unselectPiece();
	detectedActor->BeInteracted(this);
}
void APlayerCharacter::selectPiece()
{
	unselectPiece();
	detectedActor->BeInteracted(this);
}

void APlayerCharacter::placePiece()
{
	// piece enter be place into new square
	detectedActor->BeInteracted(this);

	if (selectedPiece->getPieceStatus() == EPieceStatus::EInBoard)
	{
		/* should run a timer to give few seconds before ending player turn */
		URoundManager* roundManager = URoundManager::get();
		if (roundManager)
		{
			roundManager->endCurPlayerTurn();
		}
	}
	else if (selectedPiece->getPieceStatus() == EPieceStatus::EInBench)
	{
		// nothing, just let player place any amount of piece from bench to board
	}
}

bool APlayerCharacter::isAbleToPickUpItem()
{
	return currentItemCount >= 0 && currentItemCount <= inventorySize;
}

void APlayerCharacter::unselectPiece()
{
	selectedPiece = nullptr;

	AEnvBoard* board = Cast<AGameplayGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->getBoard();
	if (board)
	{
		board->resetBoard();
	}
}

APiece* APlayerCharacter::getSelectedPiece()
{
	return selectedPiece;
}

void APlayerCharacter::attachHUDToViewport(UUserWidget* shopHUD)
{
	shopHUD->AddToViewport();
}

/* item Effect*/
void APlayerCharacter::desableItemsUsage(float duration)
{
	return;
}