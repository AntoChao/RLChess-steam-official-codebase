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


ARLGameState::ARLGameState()
{
    bReplicates = true;
}

void ARLGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARLGameState, allPlayers);

    // DOREPLIFETIME(ARLGameState, board);
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

void ARLGameState::createPlayerBody_Implementation()
{
    AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());
    
    if (curGameMode)
    {
        UFactoryPlayer* playerFac = curGameMode->playerFactoryInstance;

        if (playerFac)
        {
            AActor* createdActor = playerFac->createRLActor(TEXT("testing"), temporaryPlayerSpawnLocation, FRotator::ZeroRotator);
            APlayerCharacter* curPlayerBody = Cast<APlayerCharacter>(createdActor);

            if (curPlayerBody)
            {
                allPlayers.Add(curPlayerBody);
            }
        }
    }
}

APlayerCharacter* ARLGameState::getPlayerBody(int controllerIndex)
{
    worldIndex = controllerIndex;
    APlayerCharacter* returnPlayerBody = allPlayers[controllerIndex];
    playersReady();
    return returnPlayerBody;
}


void ARLGameState::playersReady_Implementation()
{
    AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());

    if (curGameMode)
    {
        curGameMode->startIfAllPlayerLoggedIn(allPlayers);
    }
}

void ARLGameState::setBoard_Implementation()
{
    AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());

    if (curGameMode)
    {
        board = curGameMode->serverBoard;
    }
    debugFunctionOne();
}

void ARLGameState::createBench_Implementation()
{
    for (APlayerCharacter* eachPlayer : allPlayers)
    {
        if (eachPlayer && board)
        {
            eachPlayer->setPlayerBench(board->getAllSquaresOfSpecificColor(eachPlayer->getPlayerColor()));
        }
    }
}


void ARLGameState::setShop_Implementation()
{
    AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());
    // const AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (curGameMode)
    {
        shop = curGameMode->serverShop;
    }
    debugFunctionThree();
}

void ARLGameState::closeShop_Implementation()
{
    if (shop)
    {
        shop->closeShop();
        debugFunctionFour();
    }
}