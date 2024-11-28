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
    setupSingletonClasses();
    createEnvironment();
    createPlayers();
}
void AGameplayGameMode::setupSingletonClasses()
{
    playerFactoryInstance = NewObject<UFactoryPlayer>(this, playerFactoryClass);
    environmentFactoryInstance = NewObject<UFactoryEnvironment>(this, envFactoryClass);
    itemFactoryInstance = NewObject<UFactoryItem>(this, itemFactoryClass);
    pieceFactoryInstance = NewObject<UFactoryPiece>(this, pieceFactoryClass);
}

void AGameplayGameMode::createEnvironment()
{
    if (environmentFactoryInstance)
    {
         AActor* spawnedBoard = environmentFactoryInstance->createRLActor(TEXT("Board"), boardLocation, FRotator::ZeroRotator);
         if (spawnedBoard)
         {
             gameBoard = Cast<AEnvBoard>(spawnedBoard);
         }
    }
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
                FVector spawnLocation = gameBoard->getSpawnStartPositionForPlayer(playerCounter);
                FVector direction = (boardLocation - spawnLocation).GetSafeNormal();
                FRotator rotation = direction.Rotation();

                AActor* createdActor = playerFactoryInstance->createRLActor(characterName, spawnLocation, rotation);
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

AEnvBoard* AGameplayGameMode::getBoard()
{
    return gameBoard;
}