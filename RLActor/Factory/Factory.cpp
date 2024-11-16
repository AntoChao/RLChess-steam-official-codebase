#include "Factory.h"

UFactory::URLFactory()
{
    singletonInstance = nullptr;
}

UFactory* UFactory::Get()
{
    if (!singletonInstance)
    {
        Initialize();
    }
    return singletonInstance;
}

void UFactory::Initialize()
{
    singletonInstance = NewObject<UFactory>(factoryClass);
    if (singletonInstance)
    {
        singletonInstance->AddToRoot(); // Ensure the object is not garbage collected
    }
}

IRLActor* UFactory::CreateRLActor(const FString& name)
{
    if (!GEngine || !GWorld)
    {
        UE_LOG(LogTemp, Warning, TEXT("RLFactory: Engine or World context is null!"));
        return nullptr;
    }
    /*
    // Map actor names to class types or blueprints
    TSubclassOf<ARLActor> ActorClass = nullptr;

    if (Name == "Ranger")
    {
        ActorClass = LoadClass<ARLActor>(nullptr, TEXT("/Game/Blueprints/Actors/BP_Ranger"));
    }
    else if (Name == "Samurai")
    {
        ActorClass = LoadClass<ARLActor>(nullptr, TEXT("/Game/Blueprints/Actors/BP_Samurai"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("RLFactory: No class found for actor name: %s"), *Name);
        return nullptr;
    }

    // Spawn the actor in the world
    if (ActorClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        return GWorld->SpawnActor<ARLActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    }*/

    return nullptr;
}

/*
* example usage
void SomeFunction()
{
    URLFactory* Factory = URLFactory::Get();
    FString ActorName = TEXT("ExampleActor");

    ARLActor* CreatedActor = Factory->CreateRLActor(ActorName);
    if (CreatedActor)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully created actor: %s"), *ActorName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create actor: %s"), *ActorName);
    }
}

*/