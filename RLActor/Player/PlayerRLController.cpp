#include "PlayerRLController.h"

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
	player = Cast<APlayerCharacter>(InPawn);
	if (IsLocalController() && player) {
		setupMappingContextBasedOnGameMode();
	}
}

void APlayerRLController::UnPossess() {
	Super::UnPossess(InPawn);
	setupMappingContextBasedOnGameMode();
}

void APlayerRLController::setupMappingContextBasedOnGameMode() {
	if (IsLocalController()) 
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) 
		{
			if (curInstance->getCurGameMode == EGameMode::EGameplay) {
				Subsystem->AddMappingContext(GameplayMappingContext, 0);
				bShowMouseCursor = false;
				SetInputMode(FInputModeGameOnly());
			}
			else {
				Subsystem->AddMappingContext(LobbyMappingContext, 0);
				bShowMouseCursor = true;
				SetInputMode(FInputModeUIOnly());
			}
		}
	}
}

void APlayerRLController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent)) {
		// Bind inputs based on the current context
		FName CurrentLevelName = GetWorld()->GetMapName();
		
		if (CurrentLevelName.Contains("GameplayLevel")) {
			SetupGameplayInput(EnhancedInput);
		}
		else
		{
			SetupLobbyInput(EnhancedInput);
		}
	}
}

void APlayerRLController::SetupLobbyInput(UEnhancedInputComponent* EnhancedInput) {
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInput->BindAction(goBackAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::goBackFunc);

		EnhancedInput->BindAction(cursorClickAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::cursorClickFunc);
	}
}

void APlayerRLController::SetupGameplayInput(UEnhancedInputComponent* EnhancedInput) {
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(openMenuAction, ETriggerEvent::Started, this, &ACustomPlayerController::pauseGameFunc);

		EnhancedInputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::lookFunc);

		EnhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::moveFunc);

		EnhancedInputComponent->BindAction(runAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::runFunc);
		EnhancedInputComponent->BindAction(runAction, ETriggerEvent::Completed, this, &ACustomPlayerController::runEndFunc);

		EnhancedInputComponent->BindAction(interactAction, ETriggerEvent::Started, this, &ACustomPlayerController::interactFunc);
		EnhancedInputComponent->BindAction(placePieceAction, ETriggerEvent::Started, this, &ACustomPlayerController::placePieceFunc);
	}
}

void APlayerRLController::goBackFunc(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		UMyGameInstance* gameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
		EGameMode curGameMode = gameInstance->getCurGameMode();
		switch (curGameMode)
		{
		case EGameMode::ELobby
		{
			gameInstance->openNextLevel(EGameMode::EInit);
			break;
		}
		case EGameMode::EEnd
		{
			gameInstance->openNextLevel(EGameMode::EInit);
			break;
		}
		case EGameMode::EGameplay
		case EGameMode::EInit
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
	if (IsValid(player)) {
		if (Value.Get<bool>()) {
			player->openMenu();
		}
	}
}

void APlayerRLController::lookFunc(const FInputActionValue& Value) {
	if (IsValid(player)) {
		FVector2D lookVector = Value.Get<FVector2D>();
		player->look(lookVector);
	}
}

void APlayerRLController::moveFunc(const FInputActionValue& Value) {
	if (IsValid(player)) {
		FVector2D moveVector = Value.Get<FVector2D>();
		player->move(moveVector);
	}
}

void APlayerRLController::runFunc(const FInputActionValue& Value) {
	if (IsValid(player)) {
		player->run();
	}
}
void APlayerRLController::runEndFunc(const FInputActionValue& Value) {
	if (IsValid(player)) {
		player->stopRun();
	}
}

void APlayerRLController::interactFunc(const FInputActionValue& Value) {
	if (IsValid(player)) {
		// player is able to interact or not is responsability of player
		player->interactEnvironment();
	}
}

void APlayerRLController::interactFunc(const FInputActionValue& Value) {
	if (IsValid(player)) {
		player->placePiece();
	}
}