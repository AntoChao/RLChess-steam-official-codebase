// Copyright Epic Games, Inc. All Rights Reserved.

#include "RLGameState.h"

#include "Net/UnrealNetwork.h"

#include "GameplayGameMode.h"

#include "../RLActor/Factory/FactoryPlayer.h"
#include "../RLActor/Factory/FactoryEnvironment.h"
#include "../RLActor/Factory/FactoryItem.h"
#include "../RLActor/Factory/FactoryPiece.h"

#include "../RLActor/Player/PlayerCharacter.h"
#include "../RLActor/Player/PlayerRLController.h"

#include "../RLActor/Environment/EnvBoard.h"
#include "../RLActor/Environment/EnvShop.h"

#include "../RLActor/AI/AIRLController.h"

ARLGameState::ARLGameState()
{
    bReplicates = true;
}

void ARLGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARLGameState, allPlayers);

    DOREPLIFETIME(ARLGameState, board);
    DOREPLIFETIME(ARLGameState, shop);

}

/*
Important Points
BeginPlay and Replication Timing: If you call createBoard() from BeginPlay() on the server, make sure that the clients are connected and ready to receive replicated data. Sometimes, network initialization may not be fully completed when BeginPlay() is executed, which could lead to issues if the timing is off.
Client Awareness: After the board is initialized on the server and replicated, clients will automatically receive the updated value. Ensure that the client logic listens for or checks the replicated value to update their game state or UI accordingly.
*/
void ARLGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ARLGameState::createPlayerBody_Implementation(bool isDied, int index)
{
    UE_LOG(LogTemp, Warning, TEXT("GS: create body"));
    AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());
    
    if (curGameMode)
    {
        UFactoryPlayer* playerFac = curGameMode->playerFactoryInstance;

        if (playerFac)
        {
            if (!isDied)
            {
                AActor* createdActor = playerFac->createRLActor(TEXT("testing"), temporaryPlayerSpawnLocation, FRotator::ZeroRotator);
                APlayerCharacter* curPlayerBody = Cast<APlayerCharacter>(createdActor);

                if (curPlayerBody)
                {
                    UE_LOG(LogTemp, Warning, TEXT("GS: the body is not create correctly"));
                    allPlayers.Add(curPlayerBody);
                }
            }
            else
            {
                AActor* createdActor = playerFac->createRLActor(TEXT("diedBody"), temporaryPlayerSpawnLocation, FRotator::ZeroRotator);
                APlayerCharacter* curPlayerBody = Cast<APlayerCharacter>(createdActor);

                allPlayers[index] = curPlayerBody;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("GS: the player factory is not valid"));
        }
    }
}

APlayerCharacter* ARLGameState::getPlayerBody(int controllerIndex)
{
    if (controllerIndex < allPlayers.Num())
    {
        worldIndex = controllerIndex;
        APlayerCharacter* returnPlayerBody = allPlayers[controllerIndex];
        return returnPlayerBody;
    }
    return nullptr;
}

void ARLGameState::notifyBodyCreation_Implementation()
{
    AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());

    if (curGameMode)
    {
        curGameMode->updateAllPlayersBody(allPlayers);
    }
}
void ARLGameState::playersReady_Implementation()
{
    AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());

    if (curGameMode)
    {
        curGameMode->startIfAllPlayerLoggedIn();
    }
}


void ARLGameState::spawnBoard_Implementation()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());
        if (curGameMode)
        {
            UFactoryEnvironment* boardFactory = curGameMode->environmentFactoryInstance;

            if (boardFactory)
            {
                AActor* createdActor = boardFactory->createRLActor(TEXT("Board"), boardLocation, boardRotation);
                board = Cast<AEnvBoard>(createdActor);
                
                if (board)
                {
                    board->initialized();
                }
            }
        }

    }
}
void ARLGameState::initBoard_Implementation()
{
    if (board)
    {
        board->initializeBoardColor();
    }
}

void ARLGameState::spawnShop_Implementation()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());
        if (curGameMode)
        {
            UFactoryEnvironment* shopFactory = curGameMode->environmentFactoryInstance;

            if (shopFactory)
            {
                AActor* createdActor = shopFactory->createRLActor(TEXT("Shop"), boardLocation, boardRotation);
                shop = Cast<AEnvShop>(createdActor);
            }
        }
    }
}

void ARLGameState::initShop_Implementation()
{
    if (shop)
    {
        shop->createRandomShop();
    }
}

void ARLGameState::closeShop_Implementation()
{
    if (shop)
    {
        shop->closeShop();
    }
}

TArray<APlayerCharacter*> ARLGameState::getAllPlayers() const
{
    return allPlayers;
}

void ARLGameState::initAIPlayers_Implementation(int numberOfAIPlayers)
{
    AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());

    if (curGameMode)
    {
        UFactoryPlayer* playerFac = curGameMode->playerFactoryInstance;

        if (playerFac)
        {
            for (int i = 0; i < numberOfAIPlayers; i++)
            {
                AActor* createdBodyActor = playerFac->createRLActor(TEXT("testing"), FVector(500.0f, 0.0f, 0.0f), FRotator::ZeroRotator);
                APlayerCharacter* aiBody = Cast<APlayerCharacter>(createdBodyActor);

                AActor* createdControllerActor = playerFac->createRLActor(TEXT("aiController"), FVector(500.0f, 0.0f, 0.0f), FRotator::ZeroRotator);
                AAIRLController* aiController = Cast<AAIRLController>(createdControllerActor);

                if (aiBody && aiController)
                {
                    aiController->Possess(aiBody);
                    allPlayers.Add(aiBody);
                    notifyBodyCreation();
                }
            }
        }
    }
}