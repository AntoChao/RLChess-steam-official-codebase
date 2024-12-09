#include "FactoryEnvironment.h"
#include "../RLActor.h"

#include "../Environment/EnvBoard.h"
#include "../Environment/EnvShop.h"
#include "../Environment/EnvSquare.h"

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
    else if (name == TEXT("BenchSquare"))
    {
        return GetWorld()->SpawnActor<AEnvSquare>(benchSquareClass, aLocation, aRotation);
    }
    else if (name == TEXT("Shop"))
    {
        return GetWorld()->SpawnActor<AEnvShop>(shopClass, aLocation, aRotation);
    }
    else if (name == TEXT("InvisibleSquare"))
    {
        return GetWorld()->SpawnActor<AEnvSquare>(invisibleSquare, aLocation, aRotation);
    }

    // Fall back to base class implementation
    return Super::createRLActor(name, aLocation, aRotation);
}

