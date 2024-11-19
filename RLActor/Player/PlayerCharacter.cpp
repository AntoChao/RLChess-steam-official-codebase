// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create the full-body skeletal mesh
	body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FullBodyMesh"));
	body->SetupAttachment(RootComponent);
	body->SetIsReplicated(true); // Enable replication for multiplayer
	body->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f)); // Adjust for character height
	body->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // Align with capsule

	// Create the first-person camera
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	camera->SetupAttachment(RootComponent);
	camera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // Position at eye level
	camera->bUsePawnControlRotation = true; // Rotate camera with controller

}

void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}


void APlayerCharacter::look(FVector2D lookAxisVector)
{
	if (isAbleToLook)
	{
		AddControllerYawInput(lookAxisVector.X * baseTurnRate * GetWorld()->GetDeltaSeconds());
		AddControllerPitchInput(lookAxisVector.Y * baseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void APlayerCharacter::move(FVector2D movementVector)
{
	if (isAbleToMove) {
		// find out which way is forward
		const FRotator Rotation = PlayerController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, movementVector.Y * move_XSensitivity);
		AddMovementInput(RightDirection, movementVector.X * move_YSensitivity);
	}
}

void APlayerCharacter::run() {
	if (isAbleToRun) {
		isRunning = true;
		curSpeed = runSpeed;
	}
}

void APlayerCharacter::stopRun() {
	IsRunning = false;
	curSpeed = walkSpeed:
}

void APlayerCharacter::interactEnvironment()
{
	if (isAbleToInteract) // detectedActorValid
	{
		// different interaction
		if (detectedActor ischild of piece)
		{
			selectPiece();
		}
		else if (detectedActor ischild of square && selectedPiece != nullptr)
		{
			placePieceToSquare();
		}
		else if (detectedActor ischild of shop)
		{
			shop();
		}
		else if (detectedActor ischild of item)
		{
			pickUpItem();// if isAbleToPickUp
		}
		else if (selectedItem != nullptr)
		{
			useItem();
		}

		// interaction effects
		if (! detectedActor ischild of piece)
		{
			selectedPiece = nullptr;
		}
		selectedItem = nullptr;
	}
}

void APlayerCharacter::selectPiece()
{

}

void APlayerCharacter::placePiece()
{

}

void APlayerCharacter::shop()
{

}
void APlayerCharacter::pickUpItem()
{

}

void APlayerCharacter::useItem()
{

}