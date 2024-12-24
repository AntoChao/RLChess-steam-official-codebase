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

	DOREPLIFETIME(APlayerCharacter, selectedMaterial);

	DOREPLIFETIME(APlayerCharacter, setUpTime);
	DOREPLIFETIME(APlayerCharacter, isPlayerTurn);
	DOREPLIFETIME(APlayerCharacter, playerBench);
	DOREPLIFETIME(APlayerCharacter, army);

	DOREPLIFETIME(APlayerCharacter, selectedPiece);
	// DOREPLIFETIME(APlayerCharacter, selectedSquare);
	DOREPLIFETIME(APlayerCharacter, confirmedPiece);

	DOREPLIFETIME(APlayerCharacter, curMoney);

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	inventory.SetNum(inventorySize);
	curMoney = totalMoney;

}

void APlayerCharacter::initializeMaterials()
{
	// Associate colors with materials
	colorToMaterial.Add(FColor::Silver, silverMaterial);
	colorToMaterial.Add(FColor::Red, redMaterial);
	colorToMaterial.Add(FColor::Blue, blueMaterial);
	colorToMaterial.Add(FColor::Green, greenMaterial);
	colorToMaterial.Add(FColor::Yellow, yellowMaterial);
	colorToMaterial.Add(FColor::Purple, purpleMaterial);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	initializeMaterials();

	APlayerRLController* rlController = Cast<APlayerRLController>(NewController);

	if (rlController)
	{
		FColor aColor = rlController->getPlayerColor();
		if (colorToMaterial.Contains(aColor))
		{
			selectedMaterial = colorToMaterial[aColor];

			if (GetLocalRole() == ROLE_Authority)
			{
				if (selectedMaterial)
				{
					int32 MaterialCount = GetMesh()->GetNumMaterials(); // Get the number of materials on the mesh
					for (int32 Index = 0; Index < MaterialCount; ++Index)
					{
						GetMesh()->SetMaterial(Index, selectedMaterial); // Set the material for each index
					}
				}

				GetMesh()->SetVisibility(true);
			}
		}
	}
}

void APlayerCharacter::OnRep_selectedMaterial()
{
	if (selectedMaterial)
	{
		int32 MaterialCount = GetMesh()->GetNumMaterials(); // Get the number of materials on the mesh
		for (int32 Index = 0; Index < MaterialCount; ++Index)
		{
			GetMesh()->SetMaterial(Index, selectedMaterial); // Set the material for each index
		}
	}

	GetMesh()->SetVisibility(true); // Ensure the piece is visible after setting the materials
}

void APlayerCharacter::startSetup_Implementation()
{
	setUpTime = true;
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
	army.Add(newPiece);
	for (AEnvSquare* aBenchSquare : playerBench)
	{
		if (!aBenchSquare->getIsOccupied())
		{
			newPiece->bePlaced(aBenchSquare);

			unselectPiece();
			clientResetBoard(); // unselect the piece after piece benched
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
	UE_LOG(LogTemp, Warning, TEXT("LOGG: PLAYER STARTING TURN"));
	isPlayerTurn = true;
	selectedSquare = nullptr;
	setConfirmedPiece(nullptr);
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
					/*
						setup time + hit piece -> inshop -> buy piece (select piece) -> reset selected piece
						setup time + hit piece -> same color + selected piece valid -> select piece
						setup time + hit piece -> same color + selected piece no valid -> select piece
						player turn + hit piece -> same color -> select piece
						player turn + hit piece -> different color -> attack piece -> reset selecte piece
					*/
					else if (hitActor->IsA(APiece::StaticClass()))
					{
						APiece* detectedPiece = Cast<APiece>(hitActor);
						if (setUpTime)
						{
							if (detectedPiece->getPieceStatus() == EPieceStatus::EInShop)
							{
								buyPiece(detectedPiece);
							}
							else if (detectedPiece->getPieceColor() == getPlayerColor())
							{
								if (IsValid(selectedPiece))
								{
									AEnvSquare* anotherPieceSquare = selectedPiece->getOccupiedSquare();
									if (anotherPieceSquare)
									{
										if (anotherPieceSquare->IsAbleToBeInteracted(this))
										{
											// because in setup, every same color piece is in bench
											swapPieceLocations(detectedPiece);
										}
									}
								}
								else
								{
									selectPiece(detectedPiece); // in bench selection
								}
							}
						}
						else if (isPlayerTurn)
						{
							if (detectedPiece->getPieceColor() == getPlayerColor())
							{
								selectPiece(detectedPiece);
							}
							else if (IsValid(selectedPiece) && detectedPiece->getPieceColor() != getPlayerColor())
							{
								APiece* pieceToAttack = Cast<APiece>(hitActor);
								attackPiece(pieceToAttack);
							}
						}
					}
					else if ((setUpTime || isPlayerTurn) && IsValid(selectedPiece))
					{
						if (hitActor->IsA(AEnvSquare::StaticClass()))
						{
							AEnvSquare* squareToPlace = Cast<AEnvSquare>(hitActor);
							selectPlacePieceLocationBySquare2(squareToPlace);
						}
						else if (hitActor->IsA(APiecePreviewMesh::StaticClass()))
						{
							APiecePreviewMesh* previewMeshDetected = Cast<APiecePreviewMesh>(hitActor);
							selectPlacePieceLocationByPreview(previewMeshDetected);
						}
					}
					else
					{
						unselectPiece();
						clientResetBoard();
					}
				}
				else
				{
					unselectPiece();
					clientResetBoard();
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
setup time + hit piece -> inshop -> buy piece (select piece) -> reset selected piece
setup time + hit piece -> same color + selected piece valid -> swap location -> reset selected piece
setup time + hit piece -> same color + selected piece no valid -> select piece
player turn + hit piece -> same color -> select piece
player turn + hit piece -> different color -> attack piece -> reset selecte piece
*/
void APlayerCharacter::buyPiece(APiece* detectedPiece)
{
	if (detectedPiece)
	{
		if (detectedPiece->getPieceStatus() == EPieceStatus::EInShop)
		{
			setSelectedPiece(detectedPiece); // server

			serverBuyInShopPiece(); //server
		}
	}
}
void APlayerCharacter::serverBuyInShopPiece_Implementation()
{
	if (selectedPiece)
	{
		selectedPiece->inShopInteractedEffect(this);
	}
}
void APlayerCharacter::swapPieceLocations(APiece* detectedPiece)
{
	if (detectedPiece && selectedPiece)
	{
		if (detectedPiece->getPieceStatus() == EPieceStatus::EInBench)
		{
			AEnvSquare* anotherPieceSquare = selectedPiece->getOccupiedSquare();
			if (anotherPieceSquare)
			{
				detectedPiece->bePlaced(anotherPieceSquare);
				unselectPiece();
				clientResetBoard();
			}
		}
	}
}

void APlayerCharacter::selectPiece(APiece* detectedPiece) // non rpc
{
	// unselectPiece(); // server
	clientResetBoard();

	if (detectedPiece)
	{
		setSelectedPiece(detectedPiece); // server

		switch (detectedPiece->getPieceStatus())
		{
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

void APlayerCharacter::attackPiece(APiece* pieceToAttack)
{
	if (pieceToAttack)
	{
		setSelectedSquare(pieceToAttack->getOccupiedSquare());

		clientResetBoard();
	}
}

void APlayerCharacter::selectPlacePieceLocationBySquare2(AEnvSquare* detectedSquare)
{
	if (detectedSquare)
	{
		setSelectedSquare(detectedSquare);
		if (setUpTime)
		{
			moveSelectedPiece();
		}
		clientResetBoard();
	}
}

void APlayerCharacter::selectPlacePieceLocationByPreview(APiecePreviewMesh* detectedMesh)
{
	if (detectedMesh)
	{
		AEnvSquare* detectedSquare = detectedMesh->getPreviewSquare();
		setSelectedSquare(detectedSquare);
		if (setUpTime)
		{
			moveSelectedPiece();
		}
		clientResetBoard();
	}
}

void APlayerCharacter::moveSelectedPiece_Implementation()
{
	if (selectedSquare)
	{
		// piece enter be place into new square
 		selectedSquare->BeInteracted(this);
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
APiece* APlayerCharacter::getConfirmedPiece()
{
	return confirmedPiece;
}

void APlayerCharacter::setSelectedPiece_Implementation(APiece* aPiece) // client
{
	selectedPiece = aPiece;
}
void APlayerCharacter::setConfirmedPiece_Implementation(APiece* aPiece) // client
{
	confirmedPiece = aPiece;
	selectedPiece = nullptr;
}

void APlayerCharacter::setSelectedSquare(AEnvSquare* aSquare) // non rpc
{
	if (IsLocallyControlled())
	{
		setSelectedSquareEffect(aSquare);
		setConfirmedPiece(selectedPiece);
	}
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
		selectedSquare->setConfirmedMesh(nullptr); // non rpc
	}

	if (setUpTime || isPlayerTurn)
	{
		if (aSquare && selectedPiece)
		{
			if (isPlayerTurn)
			{
				aSquare->setConfirmedMesh(selectedPiece); // non rpc
			}
		}
	}

	setSelectedSquareValue(aSquare);
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