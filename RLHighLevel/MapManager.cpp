// Copyright Epic Games, Inc. All Rights Reserved.

#include "MapManager.h"
#include "GameplayGameMode.h"
#include "../RLActor/Factory/FactoryEnvironment.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

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
    // createWalls();
}

void UMapManager::createWalls()
{
    if (!gameplayGameMode || !gameplayGameMode->environmentFactoryInstance) return;

    FVector boardCenter = GameBoard->GetActorLocation();
    int rowSize = GameBoard->getRowSize() + 1;
    int wallHeight = rowSize; // Assuming the walls should be as high as the rowSize plus two for the ceiling

    TArray<FVector> wallPositions = CalculateWallPositions(boardCenter, rowSize, wallHeight);

    for (const FVector& pos : wallPositions)
    {
        AEnvSquare* wallSquare = Cast<AEnvSquare>(gameplayGameMode->environmentFactoryInstance->createRLActor(TEXT("InvisibleSquare"), pos, FRotator::ZeroRotator));
        if (wallSquare)
        {
            wallSquare->SetActorLocation(pos);
            Buildings.Add(wallSquare); // Assuming you store all structural components like this
        }
    }
}

TArray<FVector> UMapManager::CalculateWallPositions(const FVector& center, int rowSize, int wallHeight)
{
    TArray<FVector> positions;
    float squareLength = GameBoard->getSquareLength();  // Assuming square length is consistent

    // Adjust start points to align exactly with the board's edges
    int halfRowSize = rowSize / 2.0f;
    int halfSquareLength = squareLength * 0.5;
    FVector northStart = center - FVector((halfRowSize * squareLength) - halfSquareLength, (halfRowSize * squareLength) + halfSquareLength, 0);
    FVector southStart = center + FVector(-halfRowSize * squareLength, (halfRowSize * squareLength) - (halfSquareLength), 0);
    FVector eastStart = center + FVector((halfRowSize * squareLength) - (halfSquareLength), -halfRowSize * squareLength, 0);
    FVector westStart = center - FVector((halfRowSize * squareLength) + (halfSquareLength), halfRowSize * squareLength, 0);

    DrawDebugPoint(GetWorld(), northStart, 600.0f, FColor::Red, false, 50.0f);

    // Generate wall squares for each side
    for (int i = 0; i < rowSize; i++)
    {
        for (int j = 0; j < wallHeight; j++)
        {
            // North wall
            positions.Add(northStart + FVector(i * squareLength, 0, j * squareLength));
            // South wall
            positions.Add(southStart + FVector(i * squareLength, 0, j * squareLength));
            // East wall
            positions.Add(eastStart + FVector(0, i * squareLength, j * squareLength));
            // West wall
            positions.Add(westStart + FVector(0, i * squareLength, j * squareLength));
        }
    }
    return positions;
}