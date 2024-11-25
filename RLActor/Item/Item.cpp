// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item.h"

AItem::AItem()
{
	itemBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemBody"));
    SetRootComponent(itemBody);

	itemBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	itemBody->SetCollisionResponseToAllChannels(ECR_Block);
	itemBody->SetCollisionObjectType(ECC_WorldDynamic);

	itemBody->SetVisibility(true);
}

FString AItem::GetActorName()
{
	return itemName;
}

FString AItem::GetDescription()
{
	return itemDescription;
}

bool AItem::IsAbleToBeInteracted(APlayerCharacter* Sender)
{
	return false;
}

void AItem::BeInteracted(APlayerCharacter* Sender) // player already manage the inventory pointer
{
	if (!bIsPickedUp)
	{
		/* manage visibility and collision*/
		bIsPickedUp = true;
		itemBody->SetVisibility(false);
		itemBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AItem::BeUnInteracted(APlayerCharacter* Sender)
{
	return;
}

/* item usage*/
bool AItem::isAbleToBeUsed(APlayerCharacter* user, TScriptInterface<IRLActor> target)
{
	return false;
}

void AItem::beUsed(APlayerCharacter* user, TScriptInterface<IRLActor> target)
{
	return;
}