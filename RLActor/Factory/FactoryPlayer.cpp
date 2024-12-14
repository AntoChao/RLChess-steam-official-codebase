#include "FactoryPlayer.h"
#include "../RLActor.h"
#include "Net/UnrealNetwork.h"

void UFactoryPlayer::spawnRLActor_Implementation(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (gameWorld)
    {
        if (name == TEXT("testing"))
        {
            UE_LOG(LogTemp, Warning, TEXT("RLFactory Player: create RLActor_Implementataion"));

            // Spawn or create a specialized actor
            createdActor = gameWorld->SpawnActor<APlayerCharacter>(merchantClass, aLocation, aRotation);
        }
    }
    else
    {
        createdActor = nullptr;
    }
}