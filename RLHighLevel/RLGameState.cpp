// Copyright Epic Games, Inc. All Rights Reserved.

#include "RLGameState.h"
#include "Net/UnrealNetwork.h"

ARLGameState::ARLGameState()
{
}

void ARLGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARLGameState, allPlayers);
}

void ARLGameState::BeginPlay()
{
	Super::BeginPlay();
	playerBodyIndex = 0;
}

void ARLGameState::setUpGameState()
{

}

void ARLGameState::addPlayerBody(APlayerCharacter* aBody)
{
	allPlayers.Add(aBody);
}

APlayerCharacter* ARLGameState::getCorrespondPlayerBody_Implementation()
{
	APlayerCharacter* curBody = allPlayers[playerBodyIndex];
	playerBodyIndex++;
	return curBody;
}