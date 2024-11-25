// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../RLActor.h"
#include "Item.generated.h"

class ACharacterPlayer;

UCLASS(BlueprintType, Blueprintable)
class AItem : public AActor, public IRLActor
{
	GENERATED_BODY()

public:
	AItem();

public:
	virtual FString GetActorName() override;

	virtual FString GetDescription() override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	virtual void BeInteracted(APlayerCharacter* Sender) override;

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;

/* item stats*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Stats")
	FString itemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Stats")
	FString itemDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Stats")
	UStaticMeshComponent* itemBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Stats")
	bool bIsPickedUp = false;


/* item usage*/
public:
	UFUNCTION(BlueprintCallable, Category = "Item Usage")
	bool isAbleToBeUsed(APlayerCharacter* user, TScriptInterface<IRLActor> target);

	UFUNCTION(BlueprintCallable, Category = "Item Usage")
	void beUsed(APlayerCharacter* user, TScriptInterface<IRLActor> target);
};

