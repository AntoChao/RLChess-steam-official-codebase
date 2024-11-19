// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../RLActor.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

// forward declaration
class USceneComponent;
class USkeletalMeshComponent;
class UCameraComponent;

class UInputComponent;

class UAnimMontage;
class USoundBase;

/* Debug Tool
GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Control stop use weapon secondary"));
DrawDebugPoint(GetWorld(), TargetLocation, 10.0f, FColor::Red, false, 2.0f);
DrawDebugLine(GetWorld(), ViewStart, ViewEnd, FColor::Green, false, 1, 0, 1);

FString FloatString = FString::Printf(TEXT("MyFloatValue: %.2f"), MyFloatValue);
GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FloatString);
*/


UCLASS(BlueprintType, Blueprintable)
class APlayerCharacter : public ACharacter, public IRLActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

/* character basic stats*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Body Stats")
	float BodyRadius = 42.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Body Stats")
	float BodyHalfHeight = 96.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* playerCamera;


/* RL actor functions */ 
public:

	virtual FName GetActorName() const override;

	virtual FText GetDescription() const override;

	virtual bool IsAbleToInteract(RLPlayer* Sender) const override;

	virtual void BeInteracted(RLPlayer* Sender) override;

	virtual void BeUnInteracted(RLPlayer* Sender) override;

/* controller functions*/ 
public:
	UFUNCTION(BlueprintCallable, Category = "Control")
	void openMenu(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void look(FVector2D LookAxisVector);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void move(FVector2D movementVector);

	UFUNCTION(BlueprintCallable, Category = "Run Controller")
	void run();
	UFUNCTION(BlueprintCallable, Category = "Run Controller")
	void stopRun();

	UFUNCTION(BlueprintCallable, Category = "Widget Interact Control")
	void interactEnvironment();
	
	UFUNCTION(BlueprintCallable, Category = "Widget Interact Control")
	void placePiece();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look Controller")
	bool isAbleToLook = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look Controller Stats")
	float look_XSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look Controller Stats")
	float look_YSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller")
	bool isAbleToMove = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float move_XSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float move_YSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run Controller")
	bool isRunning = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run Controller")
	bool isAbleToRun = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Interact Control")
	bool isAbleToInteract = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Interact Control")
	bool isAbleToPlacePiece = false;
};

