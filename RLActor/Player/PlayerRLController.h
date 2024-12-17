#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "../../CommonEnum.h"

#include "PlayerRLController.generated.h"

class APlayerRLState;
struct FInputActionValue;
class UEnhancedInputComponent;

UCLASS(BlueprintType, Blueprintable)
class APlayerRLController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player State")
	class APlayerRLState* rlPlayerState;

	// debug value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	int playerIndex = 0;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunction();

public:
	APlayerRLController();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
	FTimerHandle mappingContextTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
	int mappingContextTimerSegs = 3;

public:
	UFUNCTION(BlueprintCallable, Category = "Control")
		FString getPlayerName();
	UFUNCTION(BlueprintCallable, Category = "Control")
		FColor getPlayerColor();

	UFUNCTION(BlueprintCallable, Category = "Control")
		FString getCharacterName();

	UFUNCTION(BlueprintCallable, Category = "Control")
	void UnPossessEffect();

	// can not be server, because it has to be local controller
	// usage -> host -> local + authority -> none/ server/ multicast
	//		 -> client -> local + proxy -> none/ multicast
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Control")
	void setupControllerBody();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Control")
	void gameStateCreateBody();

	// possession must be done on server -> server
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Control")
	void serverPossesses(APlayerRLController* currentController);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Control")
	void setupMappingContextBasedOnGameModeMulticast();

protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	bool isLocalPlayerValid;

	// possessed player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class APlayerCharacter* rlPlayer;

	UFUNCTION()
	void validateRLPlayer();

	virtual void OnPossess(APawn* InPawn) override;

protected:
	// set up mapping context
	void setupLobbyInput(UEnhancedInputComponent* EnhancedInput);
	void setupGameplayInput(UEnhancedInputComponent* EnhancedInput);

	// set up input componnet
	virtual void SetupInputComponent() override;

	// mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* lobbyMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* gameplayMappingContext;

	// all lobby action inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* goBackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* cursorClickAction;

	// all gameplay action inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* openMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* lookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* moveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* runAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* jumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* interactAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	int curInteractionCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* selectItemOne;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* selectItemTwo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* selectItemThree;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* selectItemFour;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* selectItemFive;


	UFUNCTION(BlueprintCallable, Category = "Control")
	void voidActionFunc(const FInputActionValue& Value);

	// all lobby action functionalities
	UFUNCTION(BlueprintCallable, Category = "Control")
	void goBackFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void cursorClickFunc(const FInputActionValue& Value);

	// all gameplay action functionalities
	UFUNCTION(BlueprintCallable, Category = "Control")
	void openMenuFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void pauseGameFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void lookFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void moveFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void runFunc(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Control")
	void runEndFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void jumpFunc(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Control")
	void jumpEndFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void interactFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void selectItemOneFunc(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Control")
	void selectItemTwoFunc(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Control")
	void selectItemThreeFunc(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Control")
	void selectItemFourFunc(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Control")
	void selectItemFiveFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	bool selectItemAvailable(int num);
	UFUNCTION(BlueprintCallable, Category = "Control")
	void selectItem(int itemIndex);
};

