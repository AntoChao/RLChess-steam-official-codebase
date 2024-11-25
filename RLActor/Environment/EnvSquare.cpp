// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnvSquare.h"
#include "../Player/PlayerCharacter.h"
#include "../Piece/Piece.h"
#include "../../CommonEnum.h"


AEnvSquare::AEnvSquare()
{
	
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
}

void AEnvSquare::beOccupied(APiece* aPiece)
{
	isOccupied = true;
	occupiedPiece = aPiece;
	occupiedPiece->setPieceStatus(EPieceStatus::EInBoard);
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
}

FVector2D AEnvSquare::getSquareLocation()
{
	return squareLocation;
}

FColor AEnvSquare::getSquareColorField()
{
	return squareColorField;
}

void AEnvSquare::setIsPossibleMove(bool status)
{
	isPossibleMove = status;

	// should be highlighted or not based on the bool
}

void AEnvSquare::occupiedPieceLeaved()
{
	isOccupied = false;
	occupiedPiece = nullptr;
}