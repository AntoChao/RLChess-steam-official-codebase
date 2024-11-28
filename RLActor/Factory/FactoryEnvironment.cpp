#include "FactoryEnvironment.h"
#include "../RLActor.h"

AActor* UFactoryEnvironment::createRLActor(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (name == TEXT("Board"))
    {
        return GetWorld()->SpawnActor<AEnvBoard>(boardClass, aLocation, aRotation);
    }
    else if (name == TEXT("BlackSquare"))
    {
        return GetWorld()->SpawnActor<AEnvSquare>(blackSquareClass, aLocation, aRotation);
    }
    else if (name == TEXT("WhiteSquare"))
    {
        return GetWorld()->SpawnActor<AEnvSquare>(whiteSquareClass, aLocation, aRotation);
    }
    // Fall back to base class implementation
    return Super::createRLActor(name, aLocation, aRotation);
}

