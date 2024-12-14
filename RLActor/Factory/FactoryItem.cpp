#include "FactoryItem.h"
#include "../RLActor.h" // Include your actor class header
#include "Net/UnrealNetwork.h"

void UFactoryItem::spawnRLActor_Implementation(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (gameWorld)
    {
        if (name == TEXT("SpecialActor"))
        {
            // Spawn or create a specialized actor
            // return GetWorld()->SpawnActor<AActor>(ActorToSpawn, aLocation, aRotation);
            createdActor = nullptr;
        }
    }
    else
    {
        createdActor = nullptr;
    }
}