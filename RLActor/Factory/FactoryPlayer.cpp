#include "FactoryPlayer.h"
#include "RLActor.h" // Include your actor class header

IRLActor* UFactoryPlayer::CreateRLActor(const FString& name)
{
    if (name == TEXT("SpecialActor"))
    {
        // Spawn or create a specialized actor
        return GetWorld()->SpawnActor<ARLActor>(ARLActor::StaticClass());
    }

    // Fall back to base class implementation
    return Super::CreateRLActor(Name);
}