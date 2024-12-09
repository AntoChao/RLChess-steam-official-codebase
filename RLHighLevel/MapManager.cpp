// Copyright Epic Games, Inc. All Rights Reserved.

#include "MapManager.h"
#include "GameplayGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "../RLActor/Factory/FactoryEnvironment.h"
#include "../RLActor/Environment/EnvBoard.h"
#include "../RLActor/Player/PlayerCharacter.h"
#include "../RLActor/Environment/EnvShop.h"

UMapManager* UMapManager::Instance = nullptr;

UMapManager::UMapManager()
{
    // Constructor logic here
    Instance = nullptr;
}

UMapManager* UMapManager::get()
{
    if (!Instance)
    {
        initialize();
    }
    return Instance;
}

void UMapManager::initialize()
{
    Instance = NewObject<UMapManager>();
    if (Instance)
    {
        Instance->AddToRoot(); // Ensure the object is not garbage collected
    }
}

void UMapManager::createMap()
{
    TArray<APlayerCharacter*> allPlayers;

    URoundManager* roundManager = URoundManager::get();
    if (roundManager)
    {
        allPlayers = roundManager->getAllPlayers();

        createBoard(allPlayers);
        createBench(allPlayers);
        createBuilding();
    }
}

void UMapManager::setGameMode(AGameplayGameMode* curGameMode)
{
    gameplayGameMode = curGameMode;
}

void UMapManager::createBoard(const TArray<APlayerCharacter*>& AllPlayers)
{
    UFactoryEnvironment* environmentFactory = gameplayGameMode->environmentFactoryInstance;
    FVector boardLocation = gameplayGameMode->boardLocation;
    FRotator boardRotation = gameplayGameMode->boardRotation;

    GameBoard = Cast<AEnvBoard>(environmentFactory->createRLActor(TEXT("Board"), boardLocation, boardRotation));
    if (GameBoard)
    {
        GameBoard->initialized(AllPlayers);
    }
}

void UMapManager::createBench(const TArray<APlayerCharacter*>& AllPlayers)
{
    /*
    if (AllPlayers.Num() < 2 || AllPlayers.Num() > 4)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid number of players! Must be between 2 and 4."));
        return;
    }*/

    for (APlayerCharacter* eachPlayer : AllPlayers)
    {
        eachPlayer->setPlayerBench(GameBoard->getAllSquaresOfSpecificColor(eachPlayer->getPlayerColor()));
    }
}

void UMapManager::createShop()
{
    UFactoryEnvironment* shopFactory = gameplayGameMode->environmentFactoryInstance;
    if (!shopFactory) return;

    shop = Cast<AEnvShop>(shopFactory->createRLActor(TEXT("Shop"), gameplayGameMode->shopLocation, gameplayGameMode->shopRotation));
}

void UMapManager::closeShop()
{
    if (shop)
    {
        shop->closeShop();
    }
}

void UMapManager::createBuilding()
{
    return;
}