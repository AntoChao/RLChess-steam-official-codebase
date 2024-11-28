// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnvSquare.h"
#include "../Player/PlayerCharacter.h"
#include "../Piece/Piece.h"
#include "../../CommonEnum.h"


AEnvSquare::AEnvSquare()
{
	// Create and set up the static mesh components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	squareMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Square Mesh"));
	squareMesh->SetupAttachment(RootComponent);

	highlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Highlight Mesh"));
	highlightMesh->SetupAttachment(RootComponent);
	highlightMesh->SetVisibility(false);

}

void AEnvSquare::BeginPlay()
{
	Super::BeginPlay();

	// Set the default materials in the constructor
	initializeMaterials();

	// Initially set to transparent
	setColor(FColor::Transparent);
}

void AEnvSquare::initializeMaterials()
{
	// Associate colors with materials
	colorToMaterial.Add(FColor::Red, redMaterial);
	colorToMaterial.Add(FColor::Blue, blueMaterial);
	colorToMaterial.Add(FColor::Green, greenMaterial);
	colorToMaterial.Add(FColor::Yellow, yellowMaterial);
	colorToMaterial.Add(FColor::Purple, purpleMaterial);
	colorToMaterial.Add(FColor::Transparent, transparentMaterial);
}

void AEnvSquare::setColor(const FColor& newColor)
{
	if (colorToMaterial.Contains(newColor))
	{
		highlightMesh->SetMaterial(0, colorToMaterial[newColor]);
		highlightMesh->SetVisibility(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Square set color"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Square NOT set color"));
	}
}

FString AEnvSquare::GetActorName()
{
	return TEXT("Square");
}

FString AEnvSquare::GetDescription()
{
	return TEXT("To be moved");
}

bool AEnvSquare::IsAbleToBeInteracted(APlayerCharacter* Sender)
{
	return Sender->getPlayerColor() == squareColorField;
}

void AEnvSquare::BeInteracted(APlayerCharacter* Sender)
{
	APiece* playerSelectedPiece = Sender->getSelectedPiece();
	if (playerSelectedPiece)
	{
		playerSelectedPiece->bePlaced(this);
		beOccupied(playerSelectedPiece);
	}
}

void AEnvSquare::beOccupied(APiece* aPiece)
{
	isOccupied = true;
	occupiedPiece = aPiece;
	occupiedPiece->bePlaced(this);
}

void AEnvSquare::BeUnInteracted(APlayerCharacter* Sender)
{
	return;
}

void AEnvSquare::setSquareLocation(FVector2D aLocation)
{
	squareLocation = aLocation;
}

void AEnvSquare::setSquareColorField(FColor aColor)
{
	squareColorField = aColor;
	setColor(aColor);
}

FVector2D AEnvSquare::getSquareLocation()
{
	return squareLocation;
}

FColor AEnvSquare::getSquareColorField()
{
	return squareColorField;
}

void AEnvSquare::setIsPossibleMove(bool status, FColor pieceColor)
{
	isPossibleMove = status;

	setColor(pieceColor);
}

void AEnvSquare::occupiedPieceLeaved()
{
	isOccupied = false;
	occupiedPiece = nullptr;
}

bool AEnvSquare::getIsOccupied()
{
	return isOccupied;
}