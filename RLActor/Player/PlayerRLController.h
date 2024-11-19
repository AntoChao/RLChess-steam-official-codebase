#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerRLController.generated.h"

UCLASS(BlueprintType, Blueprintable)
class APlayerRLController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerRLController();

protected:
	virtual void BeginPlay();

	// possessed player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class APlayerCharacter* player;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void UnPossess() override;

	// set up input componnet
	virtual void SetupInputComponent() override;

	// set up mapping context
	void setupMappingContextBasedOnGameMode();

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
	class UInputAction* interactAction;

	// all lobby action functionalities
	UFUNCTION(BlueprintCallable, Category = "Control")
	void goBackFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void cursorClickFunc(const FInputActionValue& Value);

	// all gameplay action functionalities
	UFUNCTION(BlueprintCallable, Category = "Control")
	void openMenuFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void lookFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void moveFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void runFunc(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Control")
	void runEndFunc(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void interactFunc(const FInputActionValue& Value);
};

