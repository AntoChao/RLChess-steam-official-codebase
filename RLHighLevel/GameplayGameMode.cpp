// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayGameMode.h"
#include "../RLActor/Factory/FactoryPlayer.h"
#include "../RLActor/Factory/FactoryEnvironment.h"
#include "../RLActor/Player/PlayerRLController.h"

AGameplayGameMode::AGameplayGameMode() {
	roundManager = NewObject<URoundManager>();
}

void AGameplayGameMode::BeginPlay()
{
	Super::BeginPlay();
	setupGame();
    startRoundSetUp();
    startRoundGameplay();
}

void AGameplayGameMode::setupGame()
{
	createPlayers();
    createEnvironment();
}
void AGameplayGameMode::createPlayers()
{
    URLFactory* playerFactory = UFactoryPlayer::get();
    int playerCounter = 0;

    TArray<APlayerCharacter*> players;

    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {    
        APlayerRLController* PC = Cast<APlayerRLController>(Iterator->Get());
        if (PC)
        {
            FString characterName = PC->rlPlayerState->characterName;
            FVector direction = (playerLocations[playerCounter] - tableLocation).GetSafeNormal();
            FRotator rotation = direction.Rotation();

            AActor* createdActor = playerFactory->createRLActor(characterName, playerLocations[playerCounter], rotation);
            APlayerCharacter* charact = Cast<APlayerCharacter>(createdActor);
            if (charact) 
            {
                PC->Possess(charact);
                players.Add(charact);
            }
        }
        playerCounter++;
    }

    roundManager = URoundManager::get();
    roundManager->setAllPlayers(players);
}

void AGameplayGameMode::createEnvironment()
{
    
    URLFactory* environmentFactory = UFactoryEnvironment::get();
    /*
    // spawn timers
    int timerCounter = 0;

    for (APlayerCharacter* aPlayer : allPlayers)
    {
        FVector direction = (timerLocations[timerCounter] - chairLocations).GetSafeNormal();
        FRotator rotation = Direction.Rotation();

        AActor* createdActor = environmentFactory->createRLActor("Timer", timerLocations[timerCounter], rotation);
        timerCounter++;
    }*/
}

void AGameplayGameMode::startRoundSetUp()
{
    roundManager = URoundManager::get();
    roundManager->startRoundManagerSetUpRound();
}

void AGameplayGameMode::startRoundGameplay()
{
    roundManager = URoundManager::get();
    roundManager->startRoundManagerGameplayRound();
}

void AGameplayGameMode::endGameplayGameMode(APlayerCharacter* winner)
{
    return;
}