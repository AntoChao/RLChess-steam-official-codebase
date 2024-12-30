#include "FactoryPlayer.h"
#include "../RLActor.h"

#include "../Player/PlayerCharacter.h"
#include "../AI/AIRLController.h"

#include "Net/UnrealNetwork.h"

void UFactoryPlayer::spawnRLActor_Implementation(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (gameWorld)
    {
        createdActor = nullptr;
        if (name == TEXT("testing"))
        {
            createdActor = gameWorld->SpawnActor<APlayerCharacter>(merchantClass, aLocation, aRotation);
        }
        else if (name == TEXT("diedBody"))
        {
            createdActor = gameWorld->SpawnActor<APlayerCharacter>(dieClass, aLocation, aRotation);
        }
        else if (name == TEXT("aiController"))
        {
            createdActor = gameWorld->SpawnActor<AAIRLController>(aiControllerClass, aLocation, aRotation);
        }
    }
    else
    {
        createdActor = nullptr;
    }
}