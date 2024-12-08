#include "RLFactory.h"

URLFactory::URLFactory()
{
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Owner = nullptr; // No specific owner unless necessary
    SpawnParams.Instigator = nullptr;
}

AActor* URLFactory::createRLActor(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (!GEngine || !GWorld)
    {
        UE_LOG(LogTemp, Warning, TEXT("RLFactory: Engine or World context is null!"));
        return nullptr;
    }

    // Add logic to create actors here
    UE_LOG(LogTemp, Warning, TEXT("RLFactory: createRLActor called for %s"), *name);
    return nullptr;
}

AActor* URLFactory::createRandom(FVector aLocation, FRotator aRotation)
{
    UE_LOG(LogTemp, Warning, TEXT("RLFactory: createRandom called"));
    return nullptr;
}

AActor* URLFactory::createRLActorByCode(int code, FVector aLocation, FRotator aRotation)
{
    UE_LOG(LogTemp, Warning, TEXT("RLFactory: createByCode called"));
    return nullptr;
}