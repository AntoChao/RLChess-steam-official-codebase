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

    startIfAllPlayerLoggedIn();
    
}

void AGameplayGameMode::setupGame()
{
    setupSingletonClasses();
    createEnvironment();
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

APlayerCharacter* AGameplayGameMode::getPlayerBody()
{
    if (!playerFactoryInstance)
    {
        playerFactoryInstance = NewObject<UFactoryPlayer>(this, playerFactoryClass);
        playerFactoryInstance->gameWorld = GetWorld();
    }

    AActor* createdActor = playerFactoryInstance->createRLActor(TEXT("testing"), temporaryPlayerSpawnLocation, FRotator::ZeroRotator);
    APlayerCharacter* aCharacter = Cast<APlayerCharacter>(createdActor);
    
    UE_LOG(LogTemp, Warning, TEXT("XDDDDDD"));

    return aCharacter;
}

void AGameplayGameMode::addNewPlayerCharacter(APlayerController* newPlayer)
{
    UE_LOG(LogTemp, Warning, TEXT("CREATING CHARACTER FOR Player %s"), *newPlayer->GetName());
    
    FString Message = FString::Printf(TEXT("CREATING CHARACTER FOR Player %s"), *newPlayer->GetName());
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);

    APlayerRLController* rlController = Cast<APlayerRLController>(newPlayer);
    allPlayerControllers.Add(rlController);

    if (!playerFactoryInstance)
    {
        playerFactoryInstance = NewObject<UFactoryPlayer>(this, playerFactoryClass);
        playerFactoryInstance->gameWorld = GetWorld();
    }

    AActor* createdActor = playerFactoryInstance->createRLActor(TEXT("testing"), temporaryPlayerSpawnLocation, FRotator::ZeroRotator);
    APawn* rlPawn = Cast<APawn>(createdActor);
    if (rlPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("LET Player %s POSSESSES THE RLPAWN"), *newPlayer->GetName());
        Message = FString::Printf(TEXT("LET Player %s POSSESSES THE RLPAWN"), *newPlayer->GetName());
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);

        rlController->Possess(rlPawn);
        // rlController->setupMappingContextBasedOnGameMode();
    }

    roundManager = URoundManager::get();
    if (roundManager)
    {
        APlayerCharacter* rlCharact = Cast<APlayerCharacter>(createdActor);

        if (rlCharact)
        {
            roundManager->setGameMode(this);
            roundManager->addPlayers(rlCharact);
        }
    }
}

void AGameplayGameMode::createEnvironment()
{
    mapManager = UMapManager::get();
    if (mapManager)
    {
        mapManager->setGameMode(this);
        mapManager->createMap();
    }
}

void AGameplayGameMode::startIfAllPlayerLoggedIn()
{
    if (allPlayerControllers.Num() >= 2) // check if all player log, should check with game instance???
    {
        startRoundSetUp(); // set players to correct location
        startRoundGameplay(); // start game
    }
    else
    {
        UWorld* gameWorld = GetWorld();
        if (gameWorld)
        {
            gameWorld->GetTimerManager().ClearTimer(waitPlayerLogInTimerHandle);
            FTimerDelegate waitPlayerLogInTimerDel;
            waitPlayerLogInTimerDel.BindUFunction(this, FName("startIfAllPlayerLoggedIn"));
            gameWorld->GetTimerManager().SetTimer(waitPlayerLogInTimerHandle, waitPlayerLogInTimerDel, waitPlayerLogInSegs, false);
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
