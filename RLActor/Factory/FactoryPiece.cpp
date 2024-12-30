#include "FactoryPiece.h"
#include "../RLActor.h"
#include "Net/UnrealNetwork.h"

void UFactoryPiece::spawnRLActor_Implementation(const FString& name, FVector aLocation, FRotator aRotation)
{
    if (gameWorld)
    {
        if (name == TEXT("Pawn"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(pawnClass, aLocation, aRotation);
        }
        else if (name == TEXT("Rook"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(rookClass, aLocation, aRotation);
        }
        else if (name == TEXT("Knight"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(knightClass, aLocation, aRotation);
        }
        else if (name == TEXT("Bishop"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(bishopClass, aLocation, aRotation);
        }
        else if (name == TEXT("Queen"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(queenClass, aLocation, aRotation);
        }
        else if (name == TEXT("Cannon"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(cannonClass, aLocation, aRotation);
        }
        else if (name == TEXT("Ranger"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(rangerClass, aLocation, aRotation);
        }
        else if (name == TEXT("Assassin"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(assassinClass, aLocation, aRotation);
        }
        else if (name == TEXT("Mage"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(mageClass, aLocation, aRotation);
        }
        else if (name == TEXT("Samurai"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(samuraiClass, aLocation, aRotation);
        }
        else if (name == TEXT("Cultist"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(cultistClass, aLocation, aRotation);
        }
        else if (name == TEXT("Scout"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(scoutClass, aLocation, aRotation);
        }
        else if (name == TEXT("Cowboy"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(cowboyClass, aLocation, aRotation);
        }
        else if (name == TEXT("Warrior"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(warrriorClass, aLocation, aRotation);
        }
        else if (name == TEXT("Doggy"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(doggyClass, aLocation, aRotation);
        }
        else if (name == TEXT("Doctor"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(doctorClass, aLocation, aRotation);
        }
        else if (name == TEXT("Witch"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(witchClass, aLocation, aRotation);
        }
        else if (name == TEXT("Guard"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(guardClass, aLocation, aRotation);
        }
        else if (name == TEXT("Ninja"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(ninjaClass, aLocation, aRotation);
        }
        else if (name == TEXT("Catapult"))
        {
            createdActor = gameWorld->SpawnActor<APiece>(catapultClass, aLocation, aRotation);
        }
    }
    else
    {
        createdActor = nullptr;
    }
}

AActor* UFactoryPiece::createRandom(FVector aLocation, FRotator aRotation)
{
    int randomInt = FMath::RandRange(1, totalPiecesClassNum);
    
    return createRLActorByCode(randomInt, aLocation, aRotation);
}

AActor* UFactoryPiece::createRLActorByCode(int code, FVector aLocation, FRotator aRotation)
{
    switch (code)
    {
    case 1:
    {
        return createRLActor(TEXT("Pawn"), aLocation, aRotation);
    }
    case 2:
    {
        return createRLActor(TEXT("Rook"), aLocation, aRotation);
    }
    case 3:
    {
        return createRLActor(TEXT("Knight"), aLocation, aRotation);
    }
    case 4:
    {
        return createRLActor(TEXT("Bishop"), aLocation, aRotation);
    }
    case 5:
    {
        return createRLActor(TEXT("Queen"), aLocation, aRotation);
    }
    case 6:
    {
        // wild card 
        return createRLActor(TEXT("Cannon"), aLocation, aRotation);
    }
    case 7:
    {
        return createRLActor(TEXT("Ranger"), aLocation, aRotation);
    }
    case 8:
    {
        return createRLActor(TEXT("Assassin"), aLocation, aRotation);
    }
    case 9:
    {
        return createRLActor(TEXT("Mage"), aLocation, aRotation);
    }
    case 10:
    {
        return createRLActor(TEXT("Samurai"), aLocation, aRotation);
    }
    case 11:
    {
        return createRLActor(TEXT("Cultist"), aLocation, aRotation);
    }
    case 12:
    {
        return createRLActor(TEXT("Scout"), aLocation, aRotation);
    }
    case 13:
    {
        return createRLActor(TEXT("Cowboy"), aLocation, aRotation);
    }
    case 14:
    {
        return createRLActor(TEXT("Warrior"), aLocation, aRotation);
    }
    case 15:
    {
        return createRLActor(TEXT("Doggy"), aLocation, aRotation);
    }
    case 16:
    {
        return createRLActor(TEXT("Doctor"), aLocation, aRotation);
    }
    case 17:
    {
        return createRLActor(TEXT("Witch"), aLocation, aRotation);
    }
    case 18:
    {
        return createRLActor(TEXT("Guard"), aLocation, aRotation);
    }
    case 19:
    {
        return createRLActor(TEXT("Ninja"), aLocation, aRotation);
    }
    case 20:
    {
        return createRLActor(TEXT("Catapult"), aLocation, aRotation);
    }
    }

    return nullptr;
}