#include "FactoryPlayer.h"
#include "RLActor.h" // Include your actor class header

AActor* UFactoryPlayer::createRLActor(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (name == TEXT("SpecialActor"))
    {
        // Spawn or create a specialized actor
        return GetWorld()->SpawnActor<AActor>(ActorToSpawn, aLocation, aRotation);
    }

    // Fall back to base class implementation
    return Super::createRLActor(name);
}