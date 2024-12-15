// Copyright Epic Games, Inc. All Rights Reserved.

#include "RLGameState.h"

#include "Net/UnrealNetwork.h"

#include "../RLActor/Factory/FactoryPlayer.h"
#include "../RLActor/Factory/FactoryEnvironment.h"
#include "../RLActor/Factory/FactoryItem.h"
#include "../RLActor/Factory/FactoryPiece.h"

#include "../RLActor/Player/PlayerCharacter.h"
#include "../RLActor/Player/PlayerRLController.h"

ARLGameState::ARLGameState()
{
}

void ARLGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARLGameState, allPlayers);

    DOREPLIFETIME(ARLGameState, playerFactoryInstance);
    DOREPLIFETIME(ARLGameState, environmentFactoryInstance);
    DOREPLIFETIME(ARLGameState, itemFactoryInstance);
    DOREPLIFETIME(ARLGameState, pieceFactoryInstance);
}

void ARLGameState::BeginPlay()
{
	Super::BeginPlay();

    curPlayerIndex = 0;

    if (GetLocalRole() == ROLE_Authority)
    {
        setUpGameState();
    }
}

void ARLGameState::setUpGameState_Implementation()
{
    setupSingletonClasses();
}

void ARLGameState::setupSingletonClasses_Implementation()
{
    if (!playerFactoryInstance)
    {
        playerFactoryInstance = NewObject<UFactoryPlayer>(this, playerFactoryClass);
        playerFactoryInstance->gameWorld = GetWorld();
    }
    if (!environmentFactoryInstance)
    {
        environmentFactoryInstance = NewObject<UFactoryEnvironment>(this, envFactoryClass);
        environmentFactoryInstance->gameWorld = GetWorld();
    }
    if (!itemFactoryInstance)
    {
        itemFactoryInstance = NewObject<UFactoryItem>(this, itemFactoryClass);
        itemFactoryInstance->gameWorld = GetWorld();
    }
    if (!pieceFactoryInstance)
    {
        pieceFactoryInstance = NewObject<UFactoryPiece>(this, pieceFactoryClass);
        pieceFactoryInstance->gameWorld = GetWorld();
    }
}

void ARLGameState::createPlayerBody_Implementation()
{
    if (!playerFactoryInstance)
    {
        playerFactoryInstance = NewObject<UFactoryPlayer>(this, playerFactoryClass);
        playerFactoryInstance->gameWorld = GetWorld();
    }

    AActor* createdActor = playerFactoryInstance->createRLActor(TEXT("testing"), temporaryPlayerSpawnLocation, FRotator::ZeroRotator);
    APlayerCharacter* curPlayerBody = Cast<APlayerCharacter>(createdActor);

    if (curPlayerBody)
    {
        allPlayers.Add(curPlayerBody);
    }
}

APlayerCharacter* ARLGameState::getPlayerBody()
{
    APlayerCharacter* returnPlayerBody = allPlayers[curPlayerIndex];
    curPlayerIndex++;
    return returnPlayerBody;
}