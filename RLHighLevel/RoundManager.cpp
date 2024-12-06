// Copyright Epic Games, Inc. All Rights Reserved.

#include "RoundManager.h"
#include "GameplayGameMode.h"
#include "Algo/Sort.h"
#include "MapManager.h"
#include "Kismet/GameplayStatics.h"

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

TArray<APlayerCharacter*> URoundManager::getAllPlayers()
{
    return allPlayers;
}

void URoundManager::startRoundManagerSetUpRound()
{
    orderPlayerBySpeed();
    setPlayerInitLocation();
    spawnShop();
}

void URoundManager::orderPlayerBySpeed()
{
    // Use Unreal's built-in sorting algorithm with a custom predicate
    Algo::Sort(allPlayers, [](const APlayerCharacter* A, const APlayerCharacter* B) {
        return A->getPlayerSpeed() > B->getPlayerSpeed();
        });
}

void URoundManager::startRoundManagerGameplayRound()
{
    startPlayerSetUpTime();
    // startTurns();
}

void URoundManager::setPlayerInitLocation()
{
    UMapManager* mapManager = UMapManager::get();
    if (mapManager)
    {
        AEnvBoard* gameBoard = mapManager->getGameBoard();
        
        for (int i = 0; i < allPlayers.Num(); i++)
        {
            FVector spawnLocation = gameBoard->getSpawnStartPositionForPlayer(i) + FVector(0.0f, 0.0f, gameplayGameMode->playerSpawnHeight);
            FRotator spawnRotation = gameBoard->getSpawnStartRotationForPlayer(i);

            allPlayers[i]->SetActorLocation(spawnLocation);
            allPlayers[i]->SetActorRotation(spawnRotation);
        }
    }
}

void URoundManager::spawnShop()
{
    UMapManager* mapManager = UMapManager::get();
    if (mapManager)
    {
        mapManager->createShop();
    }
}

// give one minutes to let player do anything they want
void URoundManager::startPlayerSetUpTime()
{
    if (gameplayGameMode)
    {
        UWorld* gameWorld = gameplayGameMode->GetWorld();
        if (gameWorld)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("START SET UP ROUND"));


            for (APlayerCharacter* eachPlayer : allPlayers)
            {
                eachPlayer->startSetup();
            }


            // Clears any existing timer to avoid multiple teleportations
            gameWorld->GetTimerManager().ClearTimer(playerSetUpTimerHandle);

            FTimerDelegate playerSetUpTimerDel;
            playerSetUpTimerDel.BindUFunction(this, FName("endPlayerSetUpTime"));
            gameWorld->GetTimerManager().SetTimer(playerSetUpTimerHandle, playerSetUpTimerDel, playerSetUpTimerSegs, false);
        }
    }
}

void URoundManager::endPlayerSetUpTime()
{
    UMapManager* mapManager = UMapManager::get();
    if (mapManager)
    {
        mapManager->closeShop();
    }
    
    // let all players know that set up time finish
    for (APlayerCharacter* eachPlayer : allPlayers)
    {
        eachPlayer->endSetup();
    }

    // start the turns
    startTurns();
}

void URoundManager::startTurns()
{
    roundCounter++;
    startNextPlayerTurn();
}

void URoundManager::startNextPlayerTurn()
{
    // check if game finished
    /* uncomment for multiplayer
    checkIfGameEnd() */

    turnCounter++;

    if (allPlayers[curPlayerIndex]->checkIsAlive())
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("START NEXT PLAYER TURN"));

        // enable player to move piece
        allPlayers[curPlayerIndex]->startTurn();
        // calculate the current turn and round
        if (turnCounter % numPlayers == 0)
        {
            roundCounter++;
            turnCounter = 0;
        }
        /*
        // turn on the timer of player
        UWorld* gameWorld = gameplayGameMode->GetWorld();
        if (gameWorld)
        {
            gameWorld->GetTimerManager().ClearTimer(playerTurnTimerHandle);
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("RESET TIMER"));

            FTimerDelegate playerTurnTimerDel;
            playerTurnTimerDel.BindUFunction(this, FName("endCurPlayerTurn"));
            gameWorld->GetTimerManager().SetTimer(playerTurnTimerHandle, playerTurnTimerDel, turnTimeInSegs, false);
        }*/
    }
    else
    {
        endCurPlayerTurn();
    }
}

void URoundManager::endCurPlayerTurn()
{
    // unenable player to move piece
    allPlayers[curPlayerIndex]->endTurn();

    // calculate the next player index and start its turn
    curPlayerIndex++;
    if (curPlayerIndex % numPlayers == 0)
    {
        curPlayerIndex = 0;
    }
    startNextPlayerTurn();
}


// should be called after player finish its move
void URoundManager::checkIfGameEnd() {
    int alivePlayerCounter = 0;
    APlayerCharacter* winner = nullptr;

    for (APlayerCharacter* aPlayer : allPlayers)
    {
        if (aPlayer->checkIsAlive())
        {
            alivePlayerCounter++;
            winner = aPlayer;
        }
    }

    if (alivePlayerCounter <= 1)
    {
        if (gameplayGameMode)
        {
            gameplayGameMode->endGameplayGameMode(winner);
        }
    }
}

bool URoundManager::getIsPlayerSetUpTime()
{
    return isPlayerSetupTime;
}