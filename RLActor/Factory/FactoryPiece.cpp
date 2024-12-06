#include "FactoryPiece.h"
#include "../RLActor.h"

AActor* UFactoryPiece::createRLActor(const FString& name, FVector aLocation, FRotator aRotation)
{
    UE_LOG(LogTemp, Warning, TEXT("STOPPPPP"));
    if (name == TEXT("Pawn"))
    {
        return GetWorld()->SpawnActor<APiece>(pawnClass, aLocation, aRotation);
    }
    else if (name == TEXT("Rook"))
    {
        return GetWorld()->SpawnActor<APiece>(rookClass, aLocation, aRotation);
    }
    else if (name == TEXT("Knight"))
    {
        return GetWorld()->SpawnActor<APiece>(knightClass, aLocation, aRotation);
    }
    else if (name == TEXT("Bishop"))
    {
        return GetWorld()->SpawnActor<APiece>(bishopClass, aLocation, aRotation);
    }
    else if (name == TEXT("Queen"))
    {
        return GetWorld()->SpawnActor<APiece>(queenClass, aLocation, aRotation);
    }
    else if (name == TEXT("Cannon"))
    {
        return GetWorld()->SpawnActor<APiece>(cannonClass, aLocation, aRotation);
    }
    else if (name == TEXT("Ranger"))
    {
        return GetWorld()->SpawnActor<APiece>(rangerClass, aLocation, aRotation);
    }
    else if (name == TEXT("Assassin"))
    {
        return GetWorld()->SpawnActor<APiece>(assassinClass, aLocation, aRotation);
    }
    else if (name == TEXT("Mage"))
    {
        return GetWorld()->SpawnActor<APiece>(mageClass, aLocation, aRotation);
    }
    else if (name == TEXT("Samurai"))
    {
        return GetWorld()->SpawnActor<APiece>(samuraiClass, aLocation, aRotation);
    }
    else if (name == TEXT("Cultist"))
    {
        return GetWorld()->SpawnActor<APiece>(cultistClass, aLocation, aRotation);
    }
    else if (name == TEXT("Scout"))
    {
        return GetWorld()->SpawnActor<APiece>(scoutClass, aLocation, aRotation);
    }
    else if (name == TEXT("Cowboy"))
    {
        return GetWorld()->SpawnActor<APiece>(cowboyClass, aLocation, aRotation);
    }
    else if (name == TEXT("Warrior"))
    {
        return GetWorld()->SpawnActor<APiece>(warrriorClass, aLocation, aRotation);
    }
    else if (name == TEXT("Doggy"))
    {
        return GetWorld()->SpawnActor<APiece>(doggyClass, aLocation, aRotation);
    }
    else if (name == TEXT("Doctor"))
    {
        return GetWorld()->SpawnActor<APiece>(doctorClass, aLocation, aRotation);
    }
    else if (name == TEXT("Witch"))
    {
        return GetWorld()->SpawnActor<APiece>(witchClass, aLocation, aRotation);
    }
    else if (name == TEXT("Guard"))
    {
        return GetWorld()->SpawnActor<APiece>(guardClass, aLocation, aRotation);
    }
    else if (name == TEXT("Ninja"))
    {
        return GetWorld()->SpawnActor<APiece>(ninjaClass, aLocation, aRotation);
    }
    else if (name == TEXT("Catapult"))
    {
        return GetWorld()->SpawnActor<APiece>(catapultClass, aLocation, aRotation);
    }
    // Fall back to base class implementation
    return Super::createRLActor(name, aLocation, aRotation);
}

AActor* UFactoryPiece::createRandom(FVector aLocation, FRotator aRotation)
{
    int randomInt = FMath::RandRange(1, totalPiecesClassNum);
    
    switch (randomInt)
    {
        case 1:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Pawn"), aLocation, aRotation);
        }
        case 2:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Rook"), aLocation, aRotation);
        }
        case 3:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Knight"), aLocation, aRotation);
        }
        case 4:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Bishop"), aLocation, aRotation);
        }
        case 5:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Queen"), aLocation, aRotation);
        }
        case 6:
        {
            // wild card 
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Cannon"), aLocation, aRotation);
        }
        case 7:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Ranger"), aLocation, aRotation);
        }
        case 8:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Assassin"), aLocation, aRotation);
        }
        case 9:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Mage"), aLocation, aRotation);
        }
        case 10:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Samurai"), aLocation, aRotation);
        }
        case 11:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Cultist"), aLocation, aRotation);
        }
        case 12:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Scout"), aLocation, aRotation);
        }
        case 13:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Cowboy"), aLocation, aRotation);
        }
        case 14:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Warrior"), aLocation, aRotation);
        }
        case 15:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Doggy"), aLocation, aRotation);
        }
        case 16:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Doctor"), aLocation, aRotation);
        }
        case 17:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Witch"), aLocation, aRotation);
        }
        case 18:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Guard"), aLocation, aRotation);
        }
        case 19:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Ninja"), aLocation, aRotation);
        }
        case 20:
        {
            UE_LOG(LogTemp, Warning, TEXT("Generated Random Piece Index: %d"), randomInt);
            return createRLActor(TEXT("Catapult"), aLocation, aRotation);
        }
    }

    return nullptr;
}