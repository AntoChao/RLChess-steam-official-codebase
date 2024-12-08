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
	highlightMesh->SetVisibility(true);

	placeScene = CreateDefaultSubobject<USceneComponent>(TEXT("Place Scene"));
	placeScene->SetupAttachment(squareMesh);

	// Create the BoxComponent and attach it to the root
	squareCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Square Collision"));
	squareCollision->SetupAttachment(RootComponent);
	squareCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	squareCollision->SetCollisionObjectType(ECC_WorldDynamic);
	squareCollision->SetCollisionResponseToAllChannels(ECR_Overlap);

}

void AEnvSquare::BeginPlay()
{
	Super::BeginPlay();

	// Set the default materials in the constructor
	initializeMaterials();

	// Initially set to transparent
	setColor(FColor::Transparent);

	// Bind the overlap events
	if (squareCollision)
	{
		squareCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnvSquare::OnOverlapBegin);
		squareCollision->OnComponentEndOverlap.AddDynamic(this, &AEnvSquare::OnOverlapEnd);
	}
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
	// Sender->getPlayerColor() == squareColorField &&
	return isPossibleMove;
}

void AEnvSquare::BeInteracted(APlayerCharacter* Sender)
{
	APiece* playerSelectedPiece = Sender->getSelectedPiece();
	if (playerSelectedPiece)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("PLACING PLAYER SELECTED PIECE"));
		//beOccupied(playerSelectedPiece);
		playerSelectedPiece->bePlaced(this);
		
	}
}

void AEnvSquare::beOccupied(APiece* aPiece)
{
	isOccupied = true;
	occupiedPiece = aPiece;
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("BEING OCCUPIED"));
	// occupiedPiece->bePlaced(this);
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
	// setColor(aColor);
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

	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("PIECE LEAVED SQUARE"));
}

bool AEnvSquare::getIsOccupied()
{
	return isOccupied;
}

APiece* AEnvSquare::getOccupiedPiece()
{
	return occupiedPiece;
}

bool AEnvSquare::getIsPlayerOnTop()
{
	return isPlayerOnTop;
}
APlayerCharacter* AEnvSquare::getPlayerOnTop()
{
	return playerOnTop;
}

FVector AEnvSquare::getPlacementLocation()
{
	return placeScene->GetComponentLocation();
}

void AEnvSquare::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		{
			isPlayerOnTop = true;
			playerOnTop = Cast<APlayerCharacter>(OtherActor);
		}
	}
}

void AEnvSquare::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Handle overlap end
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		{
			isPlayerOnTop = false;
			playerOnTop = nullptr;
		}
	}
}