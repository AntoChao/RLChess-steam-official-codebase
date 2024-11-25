// Copyright Epic Games, Inc. All Rights Reserved.

#include "RoundManager.h"
#include "GameplayGameMode.h"
#include "Kismet/GameplayStatics.h"

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

void URoundManager::setAllPlayers(TArray<APlayerCharacter*> players)
{
    allPlayers = players;
    numPlayers = allPlayers.Num();
}

void URoundManager::startRoundManagerSetUpRound()
{
    AEnvBoard* gameBoard = AEnvBoard::get();
    gameBoard->createBoard();
    gameBoard->initializeBoardColor(allPlayers);
    gameBoard->setUpPlayerBench(allPlayers);
}

void URoundManager::startRoundManagerGameplayRound()
{
    initialFreeTime();
    startTurns();
}

// give one minutes to let player do anything they want
void URoundManager::initialFreeTime()
{
    return;
}

void URoundManager::startTurns()
{
    curPlayerIndex = findInitPlayerIndex();

    roundCounter++;
    startNextPlayerTurn();
}

int URoundManager::findInitPlayerIndex()
{
    int minSpeed = INT_MAX;
    int minSpeedIndex = 0;
    for (int i = 0; i < numPlayers; i++)
    {
        int curSpeed = allPlayers[i]->getArmySpeed();
        if (curSpeed < minSpeed)
        {
            minSpeed = curSpeed;
            minSpeedIndex = i;
        }
    }

    return minSpeedIndex;
}

void URoundManager::startNextPlayerTurn()
{
    turnCounter++;
    if (allPlayers[curPlayerIndex]->checkIsAlive())
    {
        // enable player to move piece
        allPlayers[curPlayerIndex]->startTurn();
        // calculate the current turn and round
        if (turnCounter % numPlayers == 0)
        {
            roundCounter++;
            turnCounter = 0;
        }

        // turn the timer on
        // allTimers[curPlayerIndex]->turnOn(turnTime);
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
        AGameplayGameMode* gameplayGM = Cast<AGameplayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

        if (gameplayGM)
        {
            gameplayGM->endGameplayGameMode(winner);
        }
    }
}