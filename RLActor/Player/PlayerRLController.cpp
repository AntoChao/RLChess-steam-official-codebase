#include "PlayerRLController.h"
#include "../../RLHighLevel/RLInstance.h"
#include "PlayerCharacter.h"

APlayerRLController::APlayerRLController()
{
	
}

void APlayerRLController::BeginPlay() {
	Super::BeginPlay();

	setupMappingContextBasedOnGameMode();
}

void APlayerRLController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	// Rebind input mappings if the pawn changes
	rlPlayer = Cast<APlayerCharacter>(InPawn);
	if (IsLocalController() && rlPlayer) {
		setupMappingContextBasedOnGameMode();
	}
}

void APlayerRLController::UnPossessEffect() {
	setupMappingContextBasedOnGameMode();
}

void APlayerRLController::setupMappingContextBasedOnGameMode() {
	if (IsLocalController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
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

			SetupInputComponent();
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

	EnhancedInput->BindAction(interactAction, ETriggerEvent::Started, this, &APlayerRLController::interactFunc);
		

	EnhancedInput->BindAction(selectItemOne, ETriggerEvent::Triggered, this, &APlayerRLController::selectItemOneFunc);
	EnhancedInput->BindAction(selectItemTwo, ETriggerEvent::Triggered, this, &APlayerRLController::selectItemTwoFunc);
	EnhancedInput->BindAction(selectItemThree, ETriggerEvent::Triggered, this, &APlayerRLController::selectItemThreeFunc);
	EnhancedInput->BindAction(selectItemFour, ETriggerEvent::Triggered, this, &APlayerRLController::selectItemFourFunc);
	EnhancedInput->BindAction(selectItemFive, ETriggerEvent::Triggered, this, &APlayerRLController::selectItemFiveFunc);
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
		rlPlayer->run();
	}
}
void APlayerRLController::runEndFunc(const FInputActionValue& Value) {
	if (rlPlayer) {
		rlPlayer->stopRun();
	}
}

void APlayerRLController::interactFunc(const FInputActionValue& Value) {
	if (rlPlayer) {
		// player is able to interact or not is responsability of player
		rlPlayer->interact();
	}
}

void APlayerRLController::selectItemOneFunc(const FInputActionValue& Value)
{
	if (selectItemAvailable(1))
	{
		selectItem(1);
	}
}
void APlayerRLController::selectItemTwoFunc(const FInputActionValue& Value)
{
	if (selectItemAvailable(2))
	{
		selectItem(2);
	}
}
void APlayerRLController::selectItemThreeFunc(const FInputActionValue& Value)
{
	if (selectItemAvailable(3))
	{
		selectItem(3);
	}
}
void APlayerRLController::selectItemFourFunc(const FInputActionValue& Value)
{
	if (selectItemAvailable(4))
	{
		selectItem(4);
	}
}
void APlayerRLController::selectItemFiveFunc(const FInputActionValue& Value)
{
	if (selectItemAvailable(5))
	{
		selectItem(5);
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