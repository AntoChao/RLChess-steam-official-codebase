// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayGameMode.h"
#include "../RLActor/Factory/FactoryPlayer.h"
#include "../RLActor/Factory/FactoryEnvironment.h"
#include "../RLActor/Player/PlayerRLController.h"
#include "../RLActor/Factory/FactoryItem.h"
#include "../RLActor/Factory/FactoryPiece.h"

#include "../RLActor/Player/PlayerCharacter.h"

AGameplayGameMode::AGameplayGameMode() {
	roundManager = NewObject<URoundManager>();
}

void AGameplayGameMode::BeginPlay()
{
	Super::BeginPlay();
	setupGame(); // create everything
    startRoundSetUp(); // set players to correct location
    startRoundGameplay(); // start game
}

void AGameplayGameMode::setupGame()
{
    setupSingletonClasses();
    createPlayers();
    createEnvironment();
}
void AGameplayGameMode::setupSingletonClasses()
{
    playerFactoryInstance = NewObject<UFactoryPlayer>(this, playerFactoryClass);
    environmentFactoryInstance = NewObject<UFactoryEnvironment>(this, envFactoryClass);
    itemFactoryInstance = NewObject<UFactoryItem>(this, itemFactoryClass);
    pieceFactoryInstance = NewObject<UFactoryPiece>(this, pieceFactoryClass);
}

void AGameplayGameMode::createPlayers()
{
    if (playerFactoryInstance)
    {
        int playerCounter = 0;

        TArray<APlayerCharacter*> players;

        for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
        {
            APlayerRLController* PC = Cast<APlayerRLController>(Iterator->Get());
            if (PC)
            {
                FString characterName = PC->getCharacterName();

                AActor* createdActor = playerFactoryInstance->createRLActor(characterName, FVector::ZeroVector, FRotator::ZeroRotator);
                APlayerCharacter* charact = Cast<APlayerCharacter>(createdActor);
                if (charact)
                {
                    PC->Possess(charact);
                    players.Add(charact);
                    playerCounter++;
                }
            }
        }

        roundManager = URoundManager::get();
        if (roundManager)
        {
            roundManager->setGameMode(this);
            roundManager->setAllPlayers(players);
        }
    }
}

void AGameplayGameMode::createEnvironment()
{
    mapManager = UMapManager::get();
    if (mapManager)
    {
        mapManager->setGameMode(this);
        mapManager->createMap();
    }
}

void AGameplayGameMode::startRoundSetUp()
{
    roundManager = URoundManager::get();
    if (roundManager)
    {
        roundManager->startRoundManagerSetUpRound();
    }
}

void AGameplayGameMode::startRoundGameplay()
{
    roundManager = URoundManager::get();
    if (roundManager)
    {
        roundManager->startRoundManagerGameplayRound();
    }
}

void AGameplayGameMode::endGameplayGameMode(APlayerCharacter* winner)
{
    return;
}
