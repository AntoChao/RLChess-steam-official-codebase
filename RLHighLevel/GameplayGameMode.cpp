// Copyright Epic Games, Inc. All Rights Reserved.

#include "../RLActor/Factory/FactoryPlayer.h"
#include "../RLActor/Factory/FactoryEnvironment.h"
#include "GameplayGameMode.h"

AGameplayGameMode::AGameplayGameMode() {
	roundManager = NewObject<URoundManager>();
}

void AGameplayGameMode::BeginPlay()
{
	Super::BeginPlay();
	setupGame();
	startRound();
}

void AGameplayGameMode::setupGame()
{
	createPlayers();
    createEnvironment();
}
void AGameplayGameMode::createPlayers()
{
    UFactoryPlayer* playerFactory = UFactoryPlayer::Get();
    int playerCounter = 0;

    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); ++Iterator)
    {    
        APlayerController* PC = Iterator->Get();
        if (PC)
        {
            FString characterName = PC->PlayerRLState->characterName;
            FVector direction = (playerLocations[playerCounter] - tableLocation).GetSafeNormal();
            FRotator rotation = Direction.Rotation();

            AActor* createdActor = playerFactory->createRLActor(characterName, playerLocations[playerCounter], rotation);
            ACharacter* charact = Cast<ACharacter*>createdActor;
            if (charact) {
                PC->Possess(charact);
            }
        }
        playerCounter++;
    }
}

void AGameplayGameMode::createEnvironment()
{
    UFactoryPlayer* environmentFactory = UFactoryEnvironment::Get();
    
    // spawn timers
    int timerCounter = 0;

    for (APlayerCharacter* aPlayer : allPlayers)
    {
        FVector direction = (timerLocations[timerCounter] - chairLocations).GetSafeNormal();
        FRotator rotation = Direction.Rotation();

        AActor* createdActor = environmentFactory->createRLActor("Timer", timerLocations[timerCounter], rotation);
        timerCounter++;
    }
}

void AGameplayGameMode::startRound()
{
    roundManager = URoundManager::Get();
    roundManager->startRoundManager();
}

void AGameplayGameMode::endGameplayGameMode(APlayerCharacter* winner)
{
    return;
}