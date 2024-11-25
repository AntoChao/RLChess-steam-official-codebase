#include "FactoryPlayer.h"
#include "../RLActor.h"

UFactoryPlayer::UFactoryPlayer()
{
    static ConstructorHelpers::FClassFinder<APlayerCharacter> PlayerClassFinder(TEXT("/Game/UI/MyWidgetBP"));

    if (PlayerClassFinder.Succeeded())
    {
        merchantClass = PlayerClassFinder.Class; // Assign the loaded class to your TSubclassOf
        UE_LOG(LogTemp, Log, TEXT("Successfully found class: %s"));
    }
}

AActor* UFactoryPlayer::createRLActor(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (name == TEXT("SpecialActor"))
    {
        // Spawn or create a specialized actor
        return GetWorld()->SpawnActor<APlayerCharacter>(merchantClass, aLocation, aRotation);
    }

    // Fall back to base class implementation
    return Super::createRLActor(name, aLocation, aRotation);
}