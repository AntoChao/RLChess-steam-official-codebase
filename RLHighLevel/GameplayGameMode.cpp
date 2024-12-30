// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayGameMode.h"
#include "../RLActor/Factory/FactoryPlayer.h"
#include "../RLActor/Factory/FactoryEnvironment.h"
#include "../RLActor/Factory/FactoryItem.h"
#include "../RLActor/Factory/FactoryPiece.h"
#include "../RLActor/Environment/EnvBoard.h"

#include "../RLActor/Player/PlayerCharacter.h"
#include "../RLActor/Player/PlayerRLController.h"

#include "../RLActor/AI/AIRLController.h"
#include "RLGameState.h"

AGameplayGameMode::AGameplayGameMode() {
}

void AGameplayGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    
    setupSingletonClasses();

    APlayerRLController* rlController = Cast<APlayerRLController>(NewPlayer);
    rlController->setPlayerIndex(allPlayerControllers.Num());

    allPlayerControllers.Add(rlController);

    UE_LOG(LogTemp, Warning, TEXT("GM: One player log in"))
}

void AGameplayGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    UE_LOG(LogTemp, Warning, TEXT("GM: One player log out"));
}

void AGameplayGameMode::BeginPlay()
{
	Super::BeginPlay();
    
    UE_LOG(LogTemp, Warning, TEXT("GM: begin play"));

	setupGame(); // create everything


}

void AGameplayGameMode::setupGame()
{
    setupSingletonClasses();
    spawnMap();
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

void AGameplayGameMode::spawnMap()
{
    UE_LOG(LogTemp, Warning, TEXT("map created"));
    UWorld* serverWorld = GetWorld();
    if (serverWorld)
    {
        ARLGameState* serverGameState = Cast<ARLGameState>(serverWorld->GetGameState());
        if (serverGameState)
        {
            serverGameState->spawnBoard();
            serverGameState->spawnShop();
        }
    }
}
void AGameplayGameMode::updateAllPlayersBody(TArray<APlayerCharacter*> allPlayerBody)
{
    allPlayers = allPlayerBody;
}
void AGameplayGameMode::startIfAllPlayerLoggedIn()
{
    if (allPlayers.Num() == numberOfHumanPlayers)
    {
        UE_LOG(LogTemp, Warning, TEXT("GM: all players logged in"));
        for (APlayerCharacter* eachPlayer : allPlayers)
        {
            allRestColor.Remove(eachPlayer->getPlayerColor());
        }

        initAIPlayers();

        startSetUpRound();
        startGameplayRound();
    }
}

void AGameplayGameMode::startSetUpRound()
{
    UE_LOG(LogTemp, Warning, TEXT("GM: start setup round"));

    initMap();

    setPlayerBench();
    setPlayerInitLocation();
}

void AGameplayGameMode::initAIPlayers()
{
    UWorld* serverWorld = GetWorld();
    if (serverWorld)
    {
        ARLGameState* serverGameState = Cast<ARLGameState>(serverWorld->GetGameState());
        if (serverGameState)
        {
            // serverGameState->initAIPlayers(numberOfAIPlayers);
            for (int i = 0; i < numberOfAIPlayers; i++)
            {
                AActor* createdControllerActor = playerFactoryInstance->createRLActor(TEXT("aiController"), FVector(500.0f, 0.0f, 0.0f), FRotator::ZeroRotator);
                AAIRLController* aiController = Cast<AAIRLController>(createdControllerActor);

                if (aiController)
                {
                    aiController->setIndex(allPlayerControllers.Num() + i);
                    
                    int32 randomIndex = FMath::RandRange(0, allRestColor.Num()-1);
                    FColor randomColor = allRestColor[randomIndex];
                    aiController->setAIPlayerColor(randomColor);
                    allRestColor.Remove(randomColor);

                    aiController->gameStateCreateBody();

                    if (allPlayerControllers.Num() == numberOfAIPlayers + numberOfHumanPlayers)
                    {
                        break;
                    }
                }
            }
        }
    }
}

void AGameplayGameMode::initMap()
{
    UE_LOG(LogTemp, Warning, TEXT("GM: initialize map"));
    UWorld* serverWorld = GetWorld();
    if (serverWorld)
    {
        ARLGameState* serverGameState = Cast<ARLGameState>(serverWorld->GetGameState());
        if (serverGameState)
        {
            serverGameState->initBoard();
            serverGameState->initShop();
        }
    }
}

void AGameplayGameMode::setPlayerBench()
{
    UE_LOG(LogTemp, Warning, TEXT("GM: set player bench"));

    for (APlayerCharacter* eachPlayer : allPlayers)
    {
        if (eachPlayer)
        {
            eachPlayer->setUpBench();
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
                        UE_LOG(LogTemp, Warning, TEXT("GM: Set up player location"));
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
    UE_LOG(LogTemp, Warning, TEXT("GM: start player set up time"));

    UWorld* serverWorld = GetWorld();
    if (serverWorld)
    {
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
    UE_LOG(LogTemp, Warning, TEXT("GM: end player setup time"));

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
    UE_LOG(LogTemp, Warning, TEXT("GM: start player prepare phase"));

    if (checkIfGameEnd())
    {
        endGameplayGameMode();
    }

    roundCounter++;
    UWorld* serverWorld = GetWorld();
    if (serverWorld)
    {
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
    UE_LOG(LogTemp, Warning, TEXT("GM: start pieces moving phase"));


    // unable player selecting piece
    for (APlayerCharacter* eachPlayer : allPlayers)
    {
        if (eachPlayer)
        {
            eachPlayer->endTurn();
        }
    }

    // reset all visuals
    if (UWorld* World = GetWorld())
    {
        ARLGameState* serverGameState = Cast<ARLGameState>(World->GetGameState());
        if (serverGameState)
        {
            AEnvBoard* serberBoard = serverGameState->getGameBoard();
            if (serberBoard)
            {
                serberBoard->resetConfirmedMeshBoard();
                serberBoard->resetBoard();
            }
        }
    }

    // piece moving
    for (APlayerCharacter* eachPlayer : allPlayers)
    {
        if (eachPlayer)
        {
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

    for (AController* rlController : allPlayerControllers)
    {
        APlayerRLController* aPlayer = Cast<APlayerRLController>(rlController);
        AAIRLController* aAIPlayer = Cast<AAIRLController>(rlController);
        if (aPlayer)
        {
            if (aPlayer && !aPlayer->isDied)
            {
                alivePlayerCounter++;
                winner = aPlayer;
            }
        }
        else if (aAIPlayer)
        {
            if (aAIPlayer && !aAIPlayer->isDied)
            {
                alivePlayerCounter++;
                winner = aAIPlayer;
            }
        }
        
    }

    return alivePlayerCounter <= 1;
}

void AGameplayGameMode::endGameplayGameMode()
{
    return;
}