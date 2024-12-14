// Copyright Epic Games, Inc. All Rights Reserved.

#include "RoundManager.h"
#include "GameplayGameMode.h"
#include "Algo/Sort.h"
#include "MapManager.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "../RLActor/Environment/EnvBoard.h"
#include "../RLActor/Player/PlayerCharacter.h"
#include "../RLActor/Environment/EnvShop.h"

URoundManager* URoundManager::roundManagerInstance = nullptr;

URoundManager::URoundManager(){
    roundManagerInstance = nullptr;
}

URoundManager* URoundManager::get()
{
    if (!roundManagerInstance)
    {
        initialize();
    }
    return roundManagerInstance;
}

void URoundManager::initialize()
{
    roundManagerInstance = NewObject<URoundManager>();
    if (roundManagerInstance)
    {
        roundManagerInstance->AddToRoot(); // Ensure the object is not garbage collected
    }
}

void URoundManager::setGameMode(AGameplayGameMode* curGameMode)
{
    gameplayGameMode = curGameMode;
}
void URoundManager::setAllPlayers(TArray<APlayerCharacter*> players)
{
    allPlayers = players;
    numPlayers = allPlayers.Num();
}

void URoundManager::addPlayers(APlayerCharacter* aPlayer)
{
    if (aPlayer)
    {
        allPlayers.Add(aPlayer);
        numPlayers = allPlayers.Num();
    }
}

TArray<APlayerCharacter*> URoundManager::getAllPlayers()
{
    return allPlayers;
}

void URoundManager::startRoundManagerSetUpRound_Implementation()
{
    setMapColor();
    setPlayerBench();
    setPlayerInitLocation();
    spawnShop();
}

void URoundManager::startRoundManagerGameplayRound_Implementation()
{
    startPlayerSetUpTime();
}

void URoundManager::setMapColor()
{
    UMapManager* mapManager = UMapManager::get();
    if (mapManager)
    {
        AEnvBoard* gameBoard = mapManager->getGameBoard();
        if (gameBoard)
        {
            gameBoard->initializeBoardColor(allPlayers);
            gameBoard->resetBoard();
        }
    }
}
void URoundManager::setPlayerBench()
{
    UMapManager* mapManager = UMapManager::get();
    if (mapManager)
    {
        mapManager->createBench(allPlayers);
    }
}
void URoundManager::setPlayerInitLocation_Implementation()
{
    UMapManager* mapManager = UMapManager::get();
    if (mapManager)
    {
        AEnvBoard* gameBoard = mapManager->getGameBoard();
        
        for (int i = 0; i < allPlayers.Num(); i++)
        {
            if (allPlayers[i])
            {
                FVector spawnLocation = gameBoard->getSpawnStartPositionForPlayer(i) + FVector(0.0f, 0.0f, gameplayGameMode->playerSpawnHeight);
                FRotator spawnRotation = gameBoard->getSpawnStartRotationForPlayer(i);

            
                allPlayers[i]->SetActorLocation(spawnLocation);
                allPlayers[i]->SetActorRotation(spawnRotation);
            }
        }
    }
}

void URoundManager::spawnShop_Implementation()
{
    UMapManager* mapManager = UMapManager::get();
    if (mapManager)
    {
        mapManager->createShop();
    }
}

// give one minutes to let player do anything they want
void URoundManager::startPlayerSetUpTime_Implementation()
{
    if (gameplayGameMode)
    {
        UWorld* gameWorld = gameplayGameMode->GetWorld();
        if (gameWorld)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("START SET UP ROUND"));

            for (APlayerCharacter* eachPlayer : allPlayers)
            {
                if (eachPlayer)
                {
                    eachPlayer->startSetup();
                }
            }


            // Clears any existing timer to avoid multiple teleportations
            gameWorld->GetTimerManager().ClearTimer(playerSetUpTimerHandle);

            FTimerDelegate playerSetUpTimerDel;
            playerSetUpTimerDel.BindUFunction(this, FName("endPlayerSetUpTime"));
            gameWorld->GetTimerManager().SetTimer(playerSetUpTimerHandle, playerSetUpTimerDel, playerSetUpTimerSegs, false);
        }
    }
}

void URoundManager::endPlayerSetUpTime_Implementation()
{
    UMapManager* mapManager = UMapManager::get();
    if (mapManager)
    {
        mapManager->closeShop();
    }
    
    // let all players know that set up time finish
    for (APlayerCharacter* eachPlayer : allPlayers)
    {
        if (eachPlayer)
        {
            eachPlayer->endSetup();
        }
    }

    // start the turns
    startPlayerPreparePhase();
}

void URoundManager::startPlayerPreparePhase_Implementation()
{
    roundCounter++;
    /*if (checkIfGameEnd())
    {
        gameEnd();
    }
    else*/

    if (gameplayGameMode)
    {
        UWorld* gameWorld = gameplayGameMode->GetWorld();
        if (gameWorld)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("START SET UP ROUND"));

            for (APlayerCharacter* eachPlayer : allPlayers)
            {
                if (eachPlayer)
                {
                    eachPlayer->startTurn();
                }
            }

            gameWorld->GetTimerManager().ClearTimer(playerPrepareTimerHandle);
            FTimerDelegate playerPrepareTimerDel;
            playerPrepareTimerDel.BindUFunction(this, FName("startPieceMovingPhase"));
            gameWorld->GetTimerManager().SetTimer(playerPrepareTimerHandle, playerPrepareTimerDel, playerPrepareTimerSegs, false);
        }
    }
}

void URoundManager::startPieceMovingPhase_Implementation()
{
    // let all player move their pieces
    for (APlayerCharacter* eachPlayer : allPlayers)
    {
        if (eachPlayer)
        {
            eachPlayer->endTurn();

        }
        // eachPlayer->moveSelectedPiece();
    }
    
    // run a timer to start the prepare phase
    if (gameplayGameMode)
    {
        UWorld* gameWorld = gameplayGameMode->GetWorld();
        if (gameWorld)
        {
            gameWorld->GetTimerManager().ClearTimer(piecesMovedTimerHandle);
            FTimerDelegate piecesMovedTimerDel;
            piecesMovedTimerDel.BindUFunction(this, FName("startPlayerPreparePhase"));
            gameWorld->GetTimerManager().SetTimer(piecesMovedTimerHandle, piecesMovedTimerDel, playerPrepareTimerSegs, false);
        }
    }
}

// should be called after player finish its move
bool URoundManager::checkIfGameEnd() 
{
    int alivePlayerCounter = 0;

    for (APlayerCharacter* aPlayer : allPlayers)
    {
        if (aPlayer && aPlayer->checkIsAlive())
        {
            alivePlayerCounter++;
            winner = aPlayer;
        }
    }

    return alivePlayerCounter <= 1;
}

void URoundManager::gameEnd_Implementation()
{
    if (gameplayGameMode && winner)
    {
        gameplayGameMode->endGameplayGameMode(winner);
    }
}

bool URoundManager::getIsPlayerSetUpTime()
{
    return isPlayerSetupTime;
}