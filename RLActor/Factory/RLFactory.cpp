#include "RLFactory.h"

URLFactory::URLFactory()
{
    singletonInstance = nullptr;
}

URLFactory* URLFactory::get()
{
    if (!singletonInstance)
    {
        initialize();
    }
    return singletonInstance;
}

void URLFactory::initialize()
{
    singletonInstance = NewObject<URLFactory>(factoryClass);
    if (singletonInstance)
    {
        singletonInstance->AddToRoot(); // Ensure the object is not garbage collected
    }
}

AActor* URLFactory::createRLActor(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (!GEngine || !GWorld)
    {
        UE_LOG(LogTemp, Warning, TEXT("RLFactory: Engine or World context is null!"));
        return nullptr;
    }
    return nullptr;
}

AActor* URLFactory::createRandom(FVector aLocation, FRotator aRotation)
{
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