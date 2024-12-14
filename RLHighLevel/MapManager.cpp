// Copyright Epic Games, Inc. All Rights Reserved.

#include "MapManager.h"
#include "GameplayGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

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

void UMapManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //Replicate current health.
    DOREPLIFETIME(UMapManager, GameBoard);
    DOREPLIFETIME(UMapManager, gameplayGameMode);
    DOREPLIFETIME(UMapManager, shop);
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

void UMapManager::createMap_Implementation()
{
    // TArray<APlayerCharacter*> allPlayers;

    createBoard();
    createBuilding();
}

void UMapManager::setGameMode(AGameplayGameMode* curGameMode)
{
    gameplayGameMode = curGameMode;
}

void UMapManager::createBoard_Implementation()
{
    UFactoryEnvironment* environmentFactory = gameplayGameMode->environmentFactoryInstance;
    FVector boardLocation = gameplayGameMode->boardLocation;
    FRotator boardRotation = gameplayGameMode->boardRotation;

    GameBoard = Cast<AEnvBoard>(environmentFactory->createRLActor(TEXT("Board"), boardLocation, boardRotation));
    if (GameBoard)
    {
        GameBoard->initialized();
    }
}

void UMapManager::createBench_Implementation(const TArray<APlayerCharacter*>& AllPlayers)
{
    for (APlayerCharacter* eachPlayer : AllPlayers)
    {
        if (eachPlayer)
        {
            eachPlayer->setPlayerBench(GameBoard->getAllSquaresOfSpecificColor(eachPlayer->getPlayerColor()));

        }
    }
}

void UMapManager::createShop_Implementation()
{
    UFactoryEnvironment* shopFactory = gameplayGameMode->environmentFactoryInstance;
    if (!shopFactory) return;

    shop = Cast<AEnvShop>(shopFactory->createRLActor(TEXT("Shop"), gameplayGameMode->shopLocation, gameplayGameMode->shopRotation));
}

void UMapManager::closeShop_Implementation()
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