#include "FactoryPlayer.h"
#include "../RLActor.h"
#include "Net/UnrealNetwork.h"

void UFactoryPlayer::spawnRLActor_Implementation(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (gameWorld)
    {
        if (name == TEXT("testing"))
        {
            createdActor = gameWorld->SpawnActor<APlayerCharacter>(merchantClass, aLocation, aRotation);
        }
        if (name == TEXT("diedBody"))
        {
            createdActor = gameWorld->SpawnActor<APlayerCharacter>(dieClass, aLocation, aRotation);
        }
    }
    else
    {
        createdActor = nullptr;
    }
}