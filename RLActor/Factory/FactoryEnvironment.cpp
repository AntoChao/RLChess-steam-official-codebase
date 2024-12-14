#include "FactoryEnvironment.h"
#include "../RLActor.h"
#include "Net/UnrealNetwork.h"

#include "../Environment/EnvBoard.h"
#include "../Environment/EnvShop.h"
#include "../Environment/EnvSquare.h"

void UFactoryEnvironment::spawnRLActor_Implementation(const FString& name, FVector aLocation, FRotator aRotation)
{
    UE_LOG(LogTemp, Warning, TEXT("RLFactory Environment: create RLActor_Implementataion"));

    if (gameWorld)
    {
        if (name == TEXT("Board"))
        {
            createdActor = gameWorld->SpawnActor<AEnvBoard>(boardClass, aLocation, aRotation);
        }
        else if (name == TEXT("BlackSquare"))
        {
            createdActor = gameWorld->SpawnActor<AEnvSquare>(blackSquareClass, aLocation, aRotation);
        }
        else if (name == TEXT("WhiteSquare"))
        {
            createdActor = gameWorld->SpawnActor<AEnvSquare>(whiteSquareClass, aLocation, aRotation);
        }
        else if (name == TEXT("BenchSquare"))
        {
            createdActor = gameWorld->SpawnActor<AEnvSquare>(benchSquareClass, aLocation, aRotation);
        }
        else if (name == TEXT("Shop"))
        {
            createdActor = gameWorld->SpawnActor<AEnvShop>(shopClass, aLocation, aRotation);
        }
        else if (name == TEXT("InvisibleSquare"))
        {
            createdActor = gameWorld->SpawnActor<AEnvSquare>(invisibleSquare, aLocation, aRotation);
        }
    }
    else
    {
        createdActor = nullptr;
    }
}

