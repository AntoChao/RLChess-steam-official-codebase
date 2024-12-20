#include "PlayerRLController.h"
#include "../../RLHighLevel/RLInstance.h"
#include "PlayerCharacter.h"
#include "Engine/LocalPlayer.h"

#include "Net/UnrealNetwork.h"
#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "PlayerRLState.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APlayerRLController::APlayerRLController()
{
	bReplicates = true;
}

void APlayerRLController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerRLController, playerIndex);
}

void APlayerRLController::BeginPlay() {
	Super::BeginPlay();

	// setupControllerBody();
}

void APlayerRLController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	isLocalPlayerValid = GetLocalPlayer() != nullptr;
}

void APlayerRLController::setPlayerIndex_Implementation(int curPlayerIndex)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		playerIndex = curPlayerIndex;
	}
}

FString APlayerRLController::getPlayerName()
{
	if (rlPlayerState)
	{
		return rlPlayerState->playerName;
	}
	else
	{
		return TEXT("no rl state");
	}
}
FColor APlayerRLController::getPlayerColor()
{
	if (playerIndex == 0)
	{
		return FColor::Green;
	}
	else
	{
		return FColor::Red;
	}
}

FString APlayerRLController::getCharacterName()
{
	if (!rlPlayerState)
	{
		rlPlayerState = GetPlayerState<APlayerRLState>();
	}
	return rlPlayerState->characterName;
}

void APlayerRLController::setupControllerBody_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		gameStateCreateBody();
	}
}

void APlayerRLController::gameStateCreateBody_Implementation()
{
	if (UWorld* World = GetWorld())
	{
		ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
		if (GameState)
		{
			// Successfully cast to MyCustomGameMode, now you can access its members
			GameState->createPlayerBody();

			serverPossesses(this);
			
		}
	}
}
/*SO, BODY + POSSESSION MUST BE DONE IN GAMEMODE WHICH IS INVOKED BY A SERVER RELIABLE FUNCTION*/
void APlayerRLController::serverPossesses_Implementation(APlayerRLController* currentController)
{
	APawn* controlledPawn = currentController->GetPawn();

	if (IsValid(controlledPawn))
	{
		controlledPawn->Destroy();
	}

	if (UWorld* World = GetWorld())
	{
		ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
		if (GameState)
		{
			// Successfully cast to MyCustomGameMode, now you can access its members
			rlPlayer = GameState->getPlayerBody(playerIndex);

			currentController->Possess(rlPlayer);

			setupMappingContextBasedOnGameModeMulticast();

			debugFunction(); // possess finished
			GameState->playersReady();
		}
	}
	
}

void APlayerRLController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);;
}

void APlayerRLController::UnPossessEffect() {
	setupMappingContextBasedOnGameModeMulticast();
}

void APlayerRLController::setupMappingContextBasedOnGameModeMulticast_Implementation() 
{
	if (IsLocalPlayerController())
	{
		if (GetLocalPlayer() != nullptr)
		{
			UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

			if (subsystem)
			{
				URLInstance* curGameInstance = Cast<URLInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				EGameMode curGameMode = curGameInstance->getCurGameMode();

				if (curGameMode == EGameMode::EGameplay) {
					subsystem->AddMappingContext(gameplayMappingContext, 0);
					bShowMouseCursor = false;
					SetInputMode(FInputModeGameOnly());
				}
				else if (curGameMode == EGameMode::ELobby) {
					subsystem->AddMappingContext(lobbyMappingContext, 0);
					bShowMouseCursor = true;
					SetInputMode(FInputModeUIOnly());
				}
				else // JUST FOR DEBUG
				{
					subsystem->AddMappingContext(gameplayMappingContext, 0);
					bShowMouseCursor = false;
					SetInputMode(FInputModeGameOnly());
				}

				SetupInputComponent();
			}
		}
	}
}

void APlayerRLController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent)) {
		
		URLInstance* curGameInstance = Cast<URLInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		EGameMode curGameMode = curGameInstance->getCurGameMode();

		if (curGameMode == EGameMode::EGameplay) {
			setupGameplayInput(EnhancedInput);
		}
		else if (curGameMode == EGameMode::ELobby)
		{
			setupLobbyInput(EnhancedInput);
		}
		else // JUST FOR DEBUG
		{
			setupGameplayInput(EnhancedInput);
		}
	}
}

void APlayerRLController::setupLobbyInput(UEnhancedInputComponent* EnhancedInput) {
	EnhancedInput->BindAction(goBackAction, ETriggerEvent::Triggered, this, &APlayerRLController::goBackFunc);

	EnhancedInput->BindAction(cursorClickAction, ETriggerEvent::Triggered, this, &APlayerRLController::cursorClickFunc);
}

void APlayerRLController::setupGameplayInput(UEnhancedInputComponent* EnhancedInput) {
	EnhancedInput->BindAction(openMenuAction, ETriggerEvent::Started, this, &APlayerRLController::pauseGameFunc);

	EnhancedInput->BindAction(lookAction, ETriggerEvent::Triggered, this, &APlayerRLController::lookFunc);

	EnhancedInput->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerRLController::moveFunc);

	EnhancedInput->BindAction(runAction, ETriggerEvent::Triggered, this, &APlayerRLController::runFunc);
	EnhancedInput->BindAction(runAction, ETriggerEvent::Completed, this, &APlayerRLController::runEndFunc);

	EnhancedInput->BindAction(jumpAction, ETriggerEvent::Triggered, this, &APlayerRLController::jumpFunc);
	EnhancedInput->BindAction(jumpAction, ETriggerEvent::Completed, this, &APlayerRLController::jumpEndFunc);

	EnhancedInput->BindAction(interactAction, ETriggerEvent::Started, this, &APlayerRLController::interactFunc);
	
	EnhancedInput->BindAction(selectItemOne, ETriggerEvent::Started, this, &APlayerRLController::selectItemOneFunc);
	EnhancedInput->BindAction(selectItemTwo, ETriggerEvent::Started, this, &APlayerRLController::selectItemTwoFunc);
	EnhancedInput->BindAction(selectItemThree, ETriggerEvent::Started, this, &APlayerRLController::selectItemThreeFunc);
	EnhancedInput->BindAction(selectItemFour, ETriggerEvent::Started, this, &APlayerRLController::selectItemFourFunc);
	EnhancedInput->BindAction(selectItemFive, ETriggerEvent::Started, this, &APlayerRLController::selectItemFiveFunc);
}

void APlayerRLController::voidActionFunc(const FInputActionValue& Value)
{
	return;
}

void APlayerRLController::goBackFunc(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		URLInstance* gameInstance = Cast<URLInstance>(GetWorld()->GetGameInstance());
		EGameMode curGameMode = gameInstance->getCurGameMode();
		
		switch (curGameMode)
		{
		case EGameMode::ELobby:
		{
			gameInstance->openNextLevel(EGameMode::EInit);
			break;
		}
		case EGameMode::EEnd:
		{
			gameInstance->openNextLevel(EGameMode::EInit);
			break;
		}
		case EGameMode::EGameplay:
		case EGameMode::EInit:
		default:
			break;
		}
	}
}

void APlayerRLController::cursorClickFunc(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		return;
	}
}

// gamemode controls
void APlayerRLController::validateRLPlayer()
{
	if (!rlPlayer)
	{
		APlayerCharacter* middleCharacter = Cast<APlayerCharacter>(GetPawn());
		rlPlayer = middleCharacter;
	}
}

void APlayerRLController::openMenuFunc(const FInputActionValue& Value) {
	validateRLPlayer();
	if (rlPlayer) {
		if (Value.Get<bool>()) {
			rlPlayer->openMenu(Value);
		}
	}
}

void APlayerRLController::pauseGameFunc(const FInputActionValue& Value)
{
	validateRLPlayer();
	if (rlPlayer) {
		if (Value.Get<bool>()) {
			return;
		}
	}
	return;
}

void APlayerRLController::lookFunc(const FInputActionValue& Value) {
	validateRLPlayer();
	if (rlPlayer) {
		FVector2D lookVector = Value.Get<FVector2D>();
		rlPlayer->look(lookVector);
	}
}

void APlayerRLController::moveFunc(const FInputActionValue& Value) {
	validateRLPlayer();
	if (rlPlayer) {
		FVector2D moveVector = Value.Get<FVector2D>();
		rlPlayer->move(moveVector);
	}
}

void APlayerRLController::runFunc(const FInputActionValue& Value) {
	validateRLPlayer();
	if (rlPlayer) {
		if (Value.Get<bool>())
		{
			rlPlayer->run();
		}
	}
}
void APlayerRLController::runEndFunc(const FInputActionValue& Value) 
{
	validateRLPlayer();
	if (rlPlayer) {
		rlPlayer->stopRun();
	}
}

void APlayerRLController::jumpFunc(const FInputActionValue& Value)
{
	validateRLPlayer();
	if (rlPlayer) {
		if (Value.Get<bool>())
		{
			rlPlayer->startJump();
		}
	}
}

void APlayerRLController::jumpEndFunc(const FInputActionValue& Value)
{
	validateRLPlayer();
	if (rlPlayer) {
		if (Value.Get<bool>())
		{
			rlPlayer->jumpCompleted();
		}
	}
}

void APlayerRLController::interactFunc(const FInputActionValue& Value) 
{
	validateRLPlayer();
	if (rlPlayer) {
		if (Value.Get<bool>())
		{
			if (curInteractionCount % 2 == 0)
			{
				// player is able to interact or not is responsability of player
				rlPlayer->interact();
			}
		}
	}
	curInteractionCount++;
}

void APlayerRLController::selectItemOneFunc(const FInputActionValue& Value)
{
	if (selectItemAvailable(1))
	{
		if (Value.Get<bool>())
		{
			selectItem(1);
		}
	}
}
void APlayerRLController::selectItemTwoFunc(const FInputActionValue& Value)
{
	if (selectItemAvailable(2))
	{
		if (Value.Get<bool>())
		{
			selectItem(2);
		}
	}
}
void APlayerRLController::selectItemThreeFunc(const FInputActionValue& Value)
{
	if (selectItemAvailable(3))
	{
		if (Value.Get<bool>())
		{
			selectItem(3);
		}
	}
}
void APlayerRLController::selectItemFourFunc(const FInputActionValue& Value)
{
	if (selectItemAvailable(4))
	{
		if (Value.Get<bool>())
		{
			selectItem(4);
		}
	}
}
void APlayerRLController::selectItemFiveFunc(const FInputActionValue& Value)
{
	if (selectItemAvailable(5))
	{
		if (Value.Get<bool>())
		{
			selectItem(5);
		}
	}
}

bool APlayerRLController::selectItemAvailable(int num)
{
	validateRLPlayer();
	if (rlPlayer)
	{
		return num <= rlPlayer->getInventorySize();
	}
	return false;
}

void APlayerRLController::selectItem(int itemIndex)
{
	validateRLPlayer();
	if (rlPlayer)
	{
		rlPlayer->selectItem(itemIndex);
	}
}