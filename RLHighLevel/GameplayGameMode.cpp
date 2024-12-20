// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayGameMode.h"
#include "../RLActor/Factory/FactoryPlayer.h"
#include "../RLActor/Factory/FactoryEnvironment.h"
#include "../RLActor/Factory/FactoryItem.h"
#include "../RLActor/Factory/FactoryPiece.h"
#include "../RLActor/Environment/EnvBoard.h"

#include "../RLActor/Player/PlayerCharacter.h"
#include "../RLActor/Player/PlayerRLController.h"

#include "RLGameState.h"

AGameplayGameMode::AGameplayGameMode() {
	// roundManager = NewObject<URoundManager>();
}

void AGameplayGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    UE_LOG(LogTemp, Error, TEXT("--------------------------------------"));
    UE_LOG(LogTemp, Warning, TEXT("Player %s logged in."), *NewPlayer->GetName());
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Player %s logged in."));

    APlayerRLController* rlController = Cast<APlayerRLController>(NewPlayer);
    rlController->setPlayerIndex(allPlayerControllers.Num());

    allPlayerControllers.Add(rlController);
}

void AGameplayGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Player %s logged out."));

}

void AGameplayGameMode::BeginPlay()
{
	Super::BeginPlay();
    
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("GAMEMODE BEGIN PLAY"));

	setupGame(); // create everything
}

void AGameplayGameMode::setupGame()
{
    setupSingletonClasses();
}

void AGameplayGameMode::setupSingletonClasses()
{
    if (!playerFactoryInstance)
    {
        playerFactoryInstance = NewObject<UFactoryPlayer>(this, playerFactoryClass);
        playerFactoryInstance->gameWorld = GetWorld();
    }
    if (!environmentFactoryInstance)
    {
        environmentFactoryInstance = NewObject<UFactoryEnvironment>(this, envFactoryClass);
        environmentFactoryInstance->gameWorld = GetWorld();
    }
    if (!itemFactoryInstance)
    {
        itemFactoryInstance = NewObject<UFactoryItem>(this, itemFactoryClass);
        itemFactoryInstance->gameWorld = GetWorld();
    }
    if (!pieceFactoryInstance)
    {
        pieceFactoryInstance = NewObject<UFactoryPiece>(this, pieceFactoryClass);
        pieceFactoryInstance->gameWorld = GetWorld();
    }
}

void AGameplayGameMode::startIfAllPlayerLoggedIn(TArray<APlayerCharacter*> allPlayerBody)
{
    allPlayers = allPlayerBody;

    debugFunctionOne();

    if (allPlayers.Num() == numberOfPlayers)
    {
        startSetUpRound();
        startGameplayRound();
    }
}

void AGameplayGameMode::startSetUpRound()
{
    debugFunctionTwo();

    setBoard();
    spawnShop();

    setPlayerBench();
    setPlayerInitLocation();
}

void AGameplayGameMode::setBoard()
{
    UWorld* serverWorld = GetWorld();
    if (serverWorld)
    {
        ARLGameState* serverGameState = Cast<ARLGameState>(serverWorld->GetGameState());
        if (serverGameState)
        {
            serverGameState->createBoard();
        }
    }
}
void AGameplayGameMode::spawnShop()
{
    UWorld* serverWorld = GetWorld();
    if (serverWorld)
    {
        ARLGameState* serverGameState = Cast<ARLGameState>(serverWorld->GetGameState());
        if (serverGameState)
        {
            serverGameState->createShop();
        }
    }
}

void AGameplayGameMode::setPlayerBench()
{
    for (APlayerCharacter* eachPlayer : allPlayers)
    {
        if (eachPlayer)
        {
            eachPlayer->setUpBench();
            // eachPlayer->setPlayerBench(board->getAllSquaresOfSpecificColor(eachPlayer->getPlayerColor()));
        }
    }
}
void AGameplayGameMode::setPlayerInitLocation()
{
    UWorld* serverWorld = GetWorld();
    if (serverWorld)
    {
        ARLGameState* serverGameState = Cast<ARLGameState>(serverWorld->GetGameState());
        if (serverGameState)
        {
            AEnvBoard* gameBoard = serverGameState->getGameBoard();

            if (gameBoard)
            {
                for (int i = 0; i < allPlayers.Num(); i++)
                {
                    if (allPlayers[i])
                    {
                        FVector spawnLocation = gameBoard->getSpawnStartPositionForPlayer(i) + FVector(0.0f, 0.0f, playerSpawnHeight);
                        FRotator spawnRotation = gameBoard->getSpawnStartRotationForPlayer(i);

                        allPlayers[i]->SetActorLocation(spawnLocation);
                        allPlayers[i]->SetActorRotation(spawnRotation);
                    }
                }
            }
        }
    }
}

void AGameplayGameMode::startGameplayRound()
{
    startPlayerSetUpTime();
}

// give one minutes to let player do anything they want
void AGameplayGameMode::startPlayerSetUpTime()
{
    UWorld* serverWorld = GetWorld();
    if (serverWorld)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("START SET UP ROUND"));

        for (APlayerCharacter* eachPlayer : allPlayers)
        {
            if (eachPlayer)
            {
                eachPlayer->startSetup();
            }
        }

        serverWorld->GetTimerManager().ClearTimer(playerSetUpTimerHandle);

        FTimerDelegate playerSetUpTimerDel;
        playerSetUpTimerDel.BindUFunction(this, FName("endPlayerSetUpTime"));
        serverWorld->GetTimerManager().SetTimer(playerSetUpTimerHandle, playerSetUpTimerDel, playerSetUpTimerSegs, false);
    }
}

void AGameplayGameMode::endPlayerSetUpTime()
{
    if (UWorld* World = GetWorld())
    {
        ARLGameState* serverGameState = Cast<ARLGameState>(World->GetGameState());
        if (serverGameState)
        {
            serverGameState->closeShop();

            AEnvBoard* serberBoard = serverGameState->getGameBoard();
            if (serberBoard)
            {
                serberBoard->resetBoard();
            }
        }
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

void AGameplayGameMode::startPlayerPreparePhase()
{
    if (checkIfGameEnd())
    {
        endGameplayGameMode();
    }

    roundCounter++;
    UWorld* serverWorld = GetWorld();
    if (serverWorld)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("START Chose piece movement round"));

        for (APlayerCharacter* eachPlayer : allPlayers)
        {
            if (eachPlayer)
            {
                eachPlayer->startTurn();
            }
        }

        serverWorld->GetTimerManager().ClearTimer(playerPrepareTimerHandle);
        FTimerDelegate playerPrepareTimerDel;
        playerPrepareTimerDel.BindUFunction(this, FName("startPieceMovingPhase"));
        serverWorld->GetTimerManager().SetTimer(playerPrepareTimerHandle, playerPrepareTimerDel, playerPrepareTimerSegs, false);
    }
}

void AGameplayGameMode::startPieceMovingPhase()
{
    if (UWorld* World = GetWorld())
    {
        ARLGameState* serverGameState = Cast<ARLGameState>(World->GetGameState());
        if (serverGameState)
        {
            serverGameState->closeShop();

            AEnvBoard* serberBoard = serverGameState->getGameBoard();
            if (serberBoard)
            {
                serberBoard->resetConfirmedMeshBoard();
            }
        }
    }

    // let all player move their pieces
    for (APlayerCharacter* eachPlayer : allPlayers)
    {
        if (eachPlayer)
        {
            eachPlayer->endTurn();
            eachPlayer->moveSelectedPiece();

        }
    }

    UWorld* serverWorld = GetWorld();
    if (serverWorld)
    {
        serverWorld->GetTimerManager().ClearTimer(piecesMovedTimerHandle);
        FTimerDelegate piecesMovedTimerDel;
        piecesMovedTimerDel.BindUFunction(this, FName("startPlayerPreparePhase"));
        serverWorld->GetTimerManager().SetTimer(piecesMovedTimerHandle, piecesMovedTimerDel, piecesMovedTimerSegs, false);
    }
}

// should be called after player finish its move
bool AGameplayGameMode::checkIfGameEnd()
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

void AGameplayGameMode::endGameplayGameMode()
{
    return;
}