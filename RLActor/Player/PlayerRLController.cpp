#include "PlayerRLController.h"
#include "../../RLHighLevel/RLInstance.h"
#include "PlayerCharacter.h"
#include "Engine/LocalPlayer.h"

#include "Net/UnrealNetwork.h"
#include "../../RLHighLevel/GameplayGameMode.h"

#include "PlayerRLState.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APlayerRLController::APlayerRLController()
{
	
}

void APlayerRLController::BeginPlay() {
	Super::BeginPlay();

	/*
	if (IsLocalPlayerController())
	{
		// Cast the PlayerState to custom PlayerState class
		rlPlayerState = GetPlayerState<APlayerRLState>();
		if (!rlPlayerState)
		{
			UE_LOG(LogTemp, Error, TEXT("RLCONTROLLER %s, BeginPlay, PlayerRLState not found!"), *this->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("RLCONTROLLER, BeginPlay, PlayerRLState not found!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("RLCONTROLLER %s, BeginPlay, PlayerRLState FOUND!"), *this->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("RLCONTROLLER, BeginPlay, PlayerRLState FOUND!"));
		}

		// setupControllerBody();
	}*/
}

void APlayerRLController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	isLocalPlayerValid = GetLocalPlayer() != nullptr;
}

void APlayerRLController::setupControllerBody_Implementation()
{
	if (IsLocalPlayerController())
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			// Server-specific logic here
			AGameplayGameMode* myGameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());
			if (myGameMode)
			{
				// Successfully cast to MyCustomGameMode, now you can access its members
				rlPlayer = myGameMode->getPlayerBody();

				if (rlPlayer)
				{
					Possess(rlPlayer);
					setupMappingContextBasedOnGameModeMulticast();
				}

			}
		}
		else if (GetLocalRole() == ROLE_AutonomousProxy)
		{
			// Client owning the actor
			// Server-specific logic here
			AGameplayGameMode* MyGameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());
			UE_LOG(LogTemp, Error, TEXT("MIAUMIAUMIAU"));
			if (MyGameMode)
			{
				// Successfully cast to MyCustomGameMode, now you can access its members
				rlPlayer = MyGameMode->getPlayerBody();

				if (rlPlayer)
				{
					Possess(rlPlayer);
					setupMappingContextBasedOnGameModeMulticast();
				}

			}
		}
		else if (GetLocalRole() == ROLE_SimulatedProxy)
		{
			// Other clients
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MIAUMIAUMIAU"));
	}
}

FString APlayerRLController::getPlayerName()
{
	if (rlPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("RLCONTROLLER %s, getPlayerName, PlayerRLState EXIST!"), *this->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("RLCONTROLLER, getPlayerName, PlayerRLState EXIST!"));

		return rlPlayerState->playerName;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RLCONTROLLER %s, getPlayerName, PlayerRLState NOT EXIST"), *this->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("RLCONTROLLER, getPlayerName, PlayerRLState NOT EXIST"));
		return TEXT("no rl state");
	}
}
FColor APlayerRLController::getPlayerColor()
{
	if (rlPlayerState)
	{
		return rlPlayerState->playerColor;
	}
	else
	{
		return FColor::Green;
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

void APlayerRLController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	if (GetLocalRole() == ROLE_Authority)
	{
		// Server-specific logic here
		UE_LOG(LogTemp, Warning, TEXT("Player CONTROLLER %s ON POSSESS"), *this->GetName());

		FString Message = FString::Printf(TEXT("Player CONTROLLER %s ON POSSESS"), *this->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);

		// Rebind input mappings if the pawn changes
		APlayerCharacter* aMiddlePassway = Cast<APlayerCharacter>(InPawn);

		rlPlayer = aMiddlePassway;

		if (rlPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player CONTROLLER %s POSSESSES RLPLAYER SUCCESSFUL"), *this->GetName());
			Message = FString::Printf(TEXT("Player CONTROLLER %s POSSESSES RLPLAYER SUCCESSFUL"), *this->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player CONTROLLER %s POSSESSES RLPLAYER FAIL"), *this->GetName());
			Message = FString::Printf(TEXT("Player CONTROLLER %s POSSESSES RLPLAYER FAIL"), *this->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);
		}
	}
	else if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Client owning the actor
		UE_LOG(LogTemp, Warning, TEXT("Player CONTROLLER %s POSSESSES RLPLAYER FAIL"), *this->GetName());
	
		// Server-specific logic here
		UE_LOG(LogTemp, Warning, TEXT("Player CONTROLLER %s ON POSSESS"), *this->GetName());

		FString Message = FString::Printf(TEXT("Player CONTROLLER %s ON POSSESS"), *this->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);

		// Rebind input mappings if the pawn changes
		APlayerCharacter* aMiddlePassway = Cast<APlayerCharacter>(InPawn);

		rlPlayer = aMiddlePassway;

		if (rlPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player CONTROLLER %s POSSESSES RLPLAYER SUCCESSFUL"), *this->GetName());
			Message = FString::Printf(TEXT("Player CONTROLLER %s POSSESSES RLPLAYER SUCCESSFUL"), *this->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player CONTROLLER %s POSSESSES RLPLAYER FAIL"), *this->GetName());
			Message = FString::Printf(TEXT("Player CONTROLLER %s POSSESSES RLPLAYER FAIL"), *this->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);
		}
	
	}
	else if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		// Other clients
		UE_LOG(LogTemp, Warning, TEXT("Player CONTROLLER %s POSSESSES RLPLAYER FAIL"), *this->GetName());
	}

	
}

void APlayerRLController::UnPossessEffect() {
	setupMappingContextBasedOnGameModeMulticast();
}

void APlayerRLController::setupMappingContextBasedOnGameModeMulticast_Implementation() {
	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsLocalPlayerController())
		{
			if (GetLocalPlayer() != nullptr)
			{
				UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

				UE_LOG(LogTemp, Warning, TEXT("SET UP MAPPING CONTEXT BASE ON GAMEMODE"));
				FString Message = FString::Printf(TEXT("SET UP MAPPING CONTEXT BASE ON GAMEMODE"));
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);

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
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("SET UP MAPPING CONTEXT BASE ON GAMEMODE FAIL "));
				FString Message = FString::Printf(TEXT("SET UP MAPPING CONTEXT BASE ON GAMEMODE FAIL"));
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);

			}
		}
	}
	else if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ROLE_AutonomousProxy"));
		if (GetLocalPlayer() != nullptr)
		{
			UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

			UE_LOG(LogTemp, Warning, TEXT("SET UP MAPPING CONTEXT BASE ON GAMEMODE"));
			FString Message = FString::Printf(TEXT("SET UP MAPPING CONTEXT BASE ON GAMEMODE"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);

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
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SET UP MAPPING CONTEXT BASE ON GAMEMODE FAIL "));
			FString Message = FString::Printf(TEXT("SET UP MAPPING CONTEXT BASE ON GAMEMODE FAIL"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);

		}
	
	}
	else if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ROLE_SimulatedProxy"));
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

void APlayerRLController::openMenuFunc(const FInputActionValue& Value) {
	if (rlPlayer) {
		if (Value.Get<bool>()) {
			rlPlayer->openMenu(Value);
		}
	}
}

void APlayerRLController::pauseGameFunc(const FInputActionValue& Value)
{
	if (rlPlayer) {
		if (Value.Get<bool>()) {
			return;
		}
	}
	return;
}

void APlayerRLController::lookFunc(const FInputActionValue& Value) {
	if (rlPlayer) {
		FVector2D lookVector = Value.Get<FVector2D>();
		rlPlayer->look(lookVector);
	}
}

void APlayerRLController::moveFunc(const FInputActionValue& Value) {
	if (rlPlayer) {
		FVector2D moveVector = Value.Get<FVector2D>();
		rlPlayer->move(moveVector);
	}
}

void APlayerRLController::runFunc(const FInputActionValue& Value) {
	if (rlPlayer) {
		if (Value.Get<bool>())
		{
			rlPlayer->run();
		}
	}
}
void APlayerRLController::runEndFunc(const FInputActionValue& Value) {
	if (rlPlayer) {
		rlPlayer->stopRun();
	}
}

void APlayerRLController::jumpFunc(const FInputActionValue& Value)
{
	if (rlPlayer) {
		if (Value.Get<bool>())
		{
			rlPlayer->startJump();
		}
	}
}

void APlayerRLController::jumpEndFunc(const FInputActionValue& Value)
{
	if (rlPlayer) {
		if (Value.Get<bool>())
		{
			rlPlayer->jumpCompleted();
		}
	}
}

void APlayerRLController::interactFunc(const FInputActionValue& Value) {
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
	if (rlPlayer)
	{
		return num <= rlPlayer->getInventorySize();
	}
	return false;
}

void APlayerRLController::selectItem(int itemIndex)
{
	if (rlPlayer)
	{
		rlPlayer->selectItem(itemIndex);
	}
}