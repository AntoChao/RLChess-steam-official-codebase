#include "Factory.h"

UFactory::UFactory()
{
    singletonInstance = nullptr;
}

UFactory* UFactory::get()
{
    if (!singletonInstance)
    {
        Initialize();
    }
    return singletonInstance;
}

void UFactory::initialize()
{
    singletonInstance = NewObject<UFactory>(factoryClass);
    if (singletonInstance)
    {
        singletonInstance->AddToRoot(); // Ensure the object is not garbage collected
    }
}

AActor* UFactory::createRLActor(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (!GEngine || !GWorld)
    {
        UE_LOG(LogTemp, Warning, TEXT("RLFactory: Engine or World context is null!"));
        return nullptr;
    }
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