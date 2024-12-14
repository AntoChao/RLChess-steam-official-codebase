#include "RLFactory.h"
#include "Net/UnrealNetwork.h"

URLFactory::URLFactory()
{
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    // SpawnParams.Owner = nullptr; // No specific owner unless necessary
    // SpawnParams.Instigator = nullptr;
}

void URLFactory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //Replicate current health.
    DOREPLIFETIME(URLFactory, createdActor);
}

AActor* URLFactory::createRLActor(const FString& name, FVector aLocation, FRotator aRotation)
{
    spawnRLActor(name, aLocation, aRotation);
    return createdActor;
}

void URLFactory::spawnRLActor_Implementation(const FString& name, FVector aLocation, FRotator aRotation)
{
    UE_LOG(LogTemp, Warning, TEXT("RLFactory: create RLActor_Implementataion"));

    if (!GEngine || !GWorld)
    {
        UE_LOG(LogTemp, Warning, TEXT("RLFactory: Engine or World context is null!"));
    }
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