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
	createMap();
}
void AGameplayGameMode::createPlayers()
{
    UFactoryPlayer* Factory = UFactoryPlayer::Get();

    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); ++Iterator)
    {
        APlayerController* PC = Iterator->Get();
        if (PC)
        {
            numPlayers++;

            FString characterName = PC->PlayerRLState->characterName;
            IRLActor* createdActor = Factory->CreateRLActor(ActorName);
            ACharacter* charact = Cast<ACharacter*>createdActor;
            if (charact) {
                PC->Possess(charact);
            }
        }
    }
}

void AGameplayGameMode::createMap()
{

}
void AGameplayGameMode::startRound()
{

}
void AGameplayGameMode::updateGameplayGameState()
{
	return;
}
void AGameplayGameMode::checkIsAnyWinner()
{

}
void AGameplayGameMode::endGameplayGameMode()
{

}