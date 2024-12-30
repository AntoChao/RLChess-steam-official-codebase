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

FString AItem::GetActorName(ELanguage curLanguage)
{
	return itemName;
}

FString AItem::GetDescription(ELanguage curLanguage)
{
	return itemDescription;
}

FString AItem::GetInteractionDescription(ELanguage curLanguage)
{
	return itemInteractionDesc;
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

int AItem::GetProductCost()
{
	return 0;
}

FString AItem::GetProductName()
{
	return itemName;
}

UTexture2D* AItem::GetProductImage()
{
	return nullptr;
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