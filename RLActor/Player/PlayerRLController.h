#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "../../CommonEnum.h"
#include "PlayerRLState.h"

#include "PlayerRLController.generated.h"

UCLASS(BlueprintType, Blueprintable)
class APlayerRLController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player State")
	class APlayerRLState* rlPlayerState;

public:
	APlayerRLController();

public:
	UFUNCTION(BlueprintCallable, Category = "Control")
		FString getPlayerName();
	UFUNCTION(BlueprintCallable, Category = "Control")
		FColor getPlayerColor();

	UFUNCTION(BlueprintCallable, Category = "Control")
		FString getCharacterName();

	UFUNCTION(BlueprintCallable, Category = "Control")
	void UnPossessEffect();

protected:
	virtual void BeginPlay();

	// possessed player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class APlayerCharacter* rlPlayer;

	virtual void OnPossess(APawn* InPawn) override;

protected:
	// set up mapping context
	void setupMappingContextBasedOnGameMode();
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

