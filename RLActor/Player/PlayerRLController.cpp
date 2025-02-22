#include "PlayerRLController.h"

#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"

#include "../../RLHighLevel/RLInstance.h"
#include "PlayerCharacter.h"

#include "Net/UnrealNetwork.h"
#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "RLWidget/HUDGameplay.h"

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

	DOREPLIFETIME(APlayerRLController, isDied);
	DOREPLIFETIME(APlayerRLController, playerIndex);
	DOREPLIFETIME(APlayerRLController, playerName);
}

void APlayerRLController::BeginPlay() {
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("PC: Begin play"));

	if (IsLocalPlayerController())
	{
		initName();
		setupWidget();
		setupControllerBody();
	}
}

void APlayerRLController::initName()
{
	URLInstance* gameInstance = Cast<URLInstance>(GetWorld()->GetGameInstance());

	if (gameInstance)
	{
		FString theName = gameInstance->getName();
		playerName = theName;
		UE_LOG(LogTemp, Error, TEXT("controller initName thename: %s"), *theName);
		UE_LOG(LogTemp, Error, TEXT("controller initName: %s"), *playerName);
	}
}

void APlayerRLController::setupWidget()
{
	if (IsValid(PlayerHUDClass)) {
		UE_LOG(LogTemp, Error, TEXT("PC> Player HUD Class valid"));

		PlayerHUD = CreateWidget<UHUDGameplay>(this, PlayerHUDClass);

		if (PlayerHUD)
		{
			UE_LOG(LogTemp, Error, TEXT("PC> Player HUD valid"));

			URLInstance* gameInstance = Cast<URLInstance>(GetGameInstance());
			if (gameInstance)
			{
				PlayerHUD->hudLanguage = gameInstance->getLanguage();
			}

			PlayerHUD->AddToViewport(1);

			if (PlayerHUD)
			{
				UE_LOG(LogTemp, Error, TEXT("PC> Player HUD valid 2"));
			}
		}
	}
}

void APlayerRLController::createEndGameHUD_Implementation(const FString& aWinnerName)
{
	createEndGameHUD_Multi(aWinnerName);

	UE_LOG(LogTemp, Log, TEXT("Winner: %s"), *aWinnerName);
}

void APlayerRLController::createEndGameHUD_Multi_Implementation(const FString& aWinnerName)
{
	winnerName = aWinnerName;

	UE_LOG(LogTemp, Warning, TEXT("Try creating end game hud"));

	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Log, TEXT("PC: authority EndGame winner: %s"), *aWinnerName);
	}
	else if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		UE_LOG(LogTemp, Log, TEXT("PC: simulated EndGame winner: %s"), *aWinnerName);
	}
	else if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		UE_LOG(LogTemp, Log, TEXT("PC: autonomous EndGame winner: %s"), *aWinnerName);
	}

	if (IsValid(endGameHUDClass) && !endGameHUD)
	{
		if (PlayerHUD)
		{
			PlayerHUD->RemoveFromParent();
		}
		if (menuHUD)
		{
			menuHUD->RemoveFromParent();
		}

		UE_LOG(LogTemp, Warning, TEXT("End game hud class valid"));

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		endGameHUD = CreateWidget<UUserWidget>(this, endGameHUDClass);

		if (endGameHUD)
		{
			UE_LOG(LogTemp, Warning, TEXT("End game hud create success"));

			endGameHUD->AddToViewport(10);
		}
	}
}

void APlayerRLController::createMenuHUD()
{
	if (menuHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("LOGG: Menu already exist, remove it"));

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		menuHUD->RemoveFromParent();
		// menuHUD = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LOGG: Menu no exist, try to create"));

		if (IsValid(menuHUDClass)) {
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());

			menuHUD = CreateWidget<UUserWidget>(this, menuHUDClass);

			if (menuHUD)
			{
				UE_LOG(LogTemp, Warning, TEXT("LOGG: Menu created successful"));

				menuHUD->AddToViewport(3);
			}
		}
	}
}

void APlayerRLController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	return playerName;
}
FColor APlayerRLController::getPlayerColor()
{
	if (playerIndex == 0)
	{
		return FColor::Green;
	}
	else if (playerIndex == 1)
	{
		return FColor::Red;
	}
	else
	{
		return FColor::Blue;
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

void APlayerRLController::controlledBodyDied_Implementation()
{
	if (rlPlayer)
	{
		UnPossess();
	}
	isDied = true;

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(GameMode);
	if (curGameMode)
	{
		if (curGameMode->checkIfGameEnd())
		{
			UE_LOG(LogTemp, Error, TEXT("GM: END GAME MODE"));
			curGameMode->endGameplayGameMode();
		}
		else
		{
			gameStateCreateBody();
		}
	}
}

void APlayerRLController::setupControllerBody_Implementation()
{
	gameStateCreateBody();
}

void APlayerRLController::gameStateCreateBody_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("PC: call game state to create body"));
	if (UWorld* World = GetWorld())
	{
		ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
		if (GameState)
		{
			// Successfully cast to MyCustomGameMode, now you can access its members
			GameState->createPlayerBody(isDied, playerIndex);

			serverPossesses(this);
		}
	}
}
/*SO, BODY + POSSESSION MUST BE DONE IN GAMEMODE WHICH IS INVOKED BY A SERVER RELIABLE FUNCTION*/
void APlayerRLController::serverPossesses_Implementation(APlayerRLController* currentController)
{
	UE_LOG(LogTemp, Warning, TEXT("PC: Server possess"));
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

			if (rlPlayer)
			{
				currentController->Possess(rlPlayer);

				setupMappingContextBasedOnGameModeMulticast();

				if (!isDied)
				{
					GameState->notifyBodyCreation();
					GameState->playersReady();
				}
			}
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
	UE_LOG(LogTemp, Warning, TEXT("PC: setting up mapping context"));
	if (IsLocalPlayerController())
	{
		if (GetLocalPlayer() != nullptr)
		{
			UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

			if (subsystem)
			{
				URLInstance* curGameInstance = Cast<URLInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				EGameModeEnum curGameMode = curGameInstance->getCurGameMode();

				if (curGameMode == EGameModeEnum::EGameplay) {
					subsystem->AddMappingContext(gameplayMappingContext, 0);
					bShowMouseCursor = false;
					SetInputMode(FInputModeGameOnly());
				}
				else if (curGameMode == EGameModeEnum::ELobby) {
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
		EGameModeEnum curGameMode = curGameInstance->getCurGameMode();

		if (curGameMode == EGameModeEnum::EGameplay) {
			UE_LOG(LogTemp, Warning, TEXT("PC: setup gameplay input"));
			setupGameplayInput(EnhancedInput);
		}
		else if (curGameMode == EGameModeEnum::ELobby)
		{
			UE_LOG(LogTemp, Warning, TEXT("PC: setup lobby input"));
			setupLobbyInput(EnhancedInput);
		}
		else // JUST FOR DEBUG
		{
			UE_LOG(LogTemp, Warning, TEXT("PC: setup gameplay input"));
			setupGameplayInput(EnhancedInput);
		}
	}
}

void APlayerRLController::setupLobbyInput(UEnhancedInputComponent* EnhancedInput) {
	UE_LOG(LogTemp, Warning, TEXT("PC: bind lobby input"));
	EnhancedInput->BindAction(goBackAction, ETriggerEvent::Triggered, this, &APlayerRLController::goBackFunc);

	EnhancedInput->BindAction(cursorClickAction, ETriggerEvent::Triggered, this, &APlayerRLController::cursorClickFunc);
}

void APlayerRLController::setupGameplayInput(UEnhancedInputComponent* EnhancedInput) {
	UE_LOG(LogTemp, Warning, TEXT("PC: bind gameplay input"));
	EnhancedInput->BindAction(openMenuAction, ETriggerEvent::Started, this, &APlayerRLController::openMenuFunc);

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
		EGameModeEnum curGameMode = gameInstance->getCurGameMode();
		
		switch (curGameMode)
		{
		case EGameModeEnum::ELobby:
		{
			break;
		}
		case EGameModeEnum::EEnd:
		{
			break;
		}
		case EGameModeEnum::EGameplay:
		case EGameModeEnum::EInit:
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
	UE_LOG(LogTemp, Warning, TEXT("LOGG: Try open menu"));
	if (rlPlayer) {
		if (Value.Get<bool>()) {
			// createMenuHUD();
			
			if (curMenuCount % 2 == 0)
			{
				createMenuHUD();
			}
		}
	}
	curMenuCount++;
}

void APlayerRLController::lookFunc(const FInputActionValue& Value) {
	validateRLPlayer();
	if (rlPlayer) {
		FVector2D lookVector = Value.Get<FVector2D>();
		rlPlayer->look(lookVector);
	}
}

void APlayerRLController::moveFunc(const FInputActionValue& Value) {
	if (rlPlayer) {
		FVector2D moveVector = Value.Get<FVector2D>();
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("control move"));

		rlPlayer->moveForward(moveVector.Y);
		rlPlayer->moveRight(moveVector.X);
		// rlPlayer->move(moveVector);
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
	if (rlPlayer && !isDied) {
		if (Value.Get<bool>())
		{
			if (curInteractionCount % 2 == 0)
			{
				// player is able to interact or not is responsability of player
				rlPlayer->interact();

				if (GetLocalRole() == ROLE_Authority)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Server Controller: interact"));
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Client Controller: interact"));
				}
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

void APlayerRLController::updateGameplayHUD(bool isWaitingPar, bool isSetupPar, int curMoneyPar,
	bool isAlivePar, bool isPlayerTurnPar, TScriptInterface<IRLActor> detectedActor,
	int curRestTime)
{
	if (PlayerHUD)
	{
		PlayerHUD->updateWaitingTime(isWaitingPar);
		PlayerHUD->isSetupTime = isSetupPar;
		PlayerHUD->ownerMoney = curMoneyPar;
		PlayerHUD->isAlive = isAlivePar;
		PlayerHUD->isPlayerTurn = isPlayerTurnPar;
		PlayerHUD->curDetectedActor = detectedActor;
		PlayerHUD->turnRestTime = curRestTime;
	}
}