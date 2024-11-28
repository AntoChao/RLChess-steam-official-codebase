#include "FactoryPlayer.h"
#include "../RLActor.h"

AActor* UFactoryPlayer::createRLActor(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (name == TEXT("testing"))
    {
        // Spawn or create a specialized actor
        return GetWorld()->SpawnActor<APlayerCharacter>(merchantClass, aLocation, aRotation);
    }

    // Fall back to base class implementation
    return Super::createRLActor(name, aLocation, aRotation);
}