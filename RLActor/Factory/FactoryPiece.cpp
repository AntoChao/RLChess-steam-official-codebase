#include "FactoryPiece.h"
#include "../RLActor.h"

AActor* UFactoryPiece::createRLActor(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (name == TEXT("SpecialActor"))
    {
        // Spawn or create a specialized actor
        // return GetWorld()->SpawnActor<AActor>(ActorToSpawn, aLocation, aRotation);
        return nullptr;
    }

    // Fall back to base class implementation
    return Super::createRLActor(name, aLocation, aRotation);
}

AActor* UFactoryPiece::createRandom(FVector aLocation, FRotator aRotation)
{
    return nullptr;
}