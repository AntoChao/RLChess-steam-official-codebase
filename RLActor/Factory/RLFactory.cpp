#include "RLFactory.h"
#include "Net/UnrealNetwork.h"

URLFactory::URLFactory()
{
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

AActor* URLFactory::createRLActor(const FString& name, FVector aLocation, FRotator aRotation)
{
    spawnRLActor(name, aLocation, aRotation);
    return createdActor;
}

void URLFactory::spawnRLActor_Implementation(const FString& name, FVector aLocation, FRotator aRotation)
{
}

AActor* URLFactory::createRandom(FVector aLocation, FRotator aRotation)
{
    return nullptr;
}

AActor* URLFactory::createRLActorByCode(int code, FVector aLocation, FRotator aRotation)
{
    return nullptr;
}