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

class UAnimMontage;
class USoundBase;

class APiece;

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

	/* character basic components*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float BodyRadius = 42.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float BodyHalfHeight = 96.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* body;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* camera;

	/* character basic stats*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	bool isAlibe = false;

	/* RL actor functions */
public:

	virtual FName GetActorName() const override;

	virtual FText GetDescription() const override;

	virtual bool IsAbleToInteract(RLPlayer* Sender) const override;

	virtual void BeInteracted(APlayerCharacter* Sender) override;

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;

	/* controller functions*/
public:
	UFUNCTION(BlueprintCallable, Category = "Control")
	void openMenu(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void look(FVector2D lookAxisVector);

	UFUNCTION(BlueprintCallable, Category = "Control")
	void move(FVector2D movementVector);

	UFUNCTION(BlueprintCallable, Category = "Run Controller")
	void run();
	UFUNCTION(BlueprintCallable, Category = "Run Controller")
	void stopRun();

	UFUNCTION(BlueprintCallable, Category = "Widget Interact Control")
	void interactEnvironment();
	
	UFUNCTION(BlueprintCallable, Category = "Widget Interact Control")
	void selectPiece();
	UFUNCTION(BlueprintCallable, Category = "Widget Interact Control")
	void placePiece();
	UFUNCTION(BlueprintCallable, Category = "Widget Interact Control")
	void shop();
	UFUNCTION(BlueprintCallable, Category = "Widget Interact Control")
	void pickUpItem();
	UFUNCTION(BlueprintCallable, Category = "Widget Interact Control")
	void useItem();
	

protected:
	// look stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look Controller Stats")
	bool isAbleToLook = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look Controller Stats")
	float baseTurnRate = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look Controller Stats")
	float baseLookUpRate = 45.0f;

	// move stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	bool isAbleToMove = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float move_XSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float move_YSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	bool isRunning = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	bool isAbleToRun = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float curSpeed = 0.0f;

	// interact stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	bool isAbleToInteract = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	AActor* detectedActor = nullptr; // update by camera detection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	APiece* selectedPiece = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	AItem* selectedItem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
	bool isAbleToPlacePiece = false; // update by floor detection

	// character stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float walkSpeed = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
	float runSpeed = 600.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	float range = 500.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int rangeRank = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	int initMoney = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	TArray<AActors*> inventory;
};

