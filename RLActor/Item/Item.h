// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../RLActor.h"
#include "../RLProduct.h"
#include "Item.generated.h"

class ACharacterPlayer;

UCLASS(BlueprintType, Blueprintable)
class AItem : public AActor, public IRLActor, public IRLProduct
{
	GENERATED_BODY()

public:
	AItem();

public:
	virtual FString GetActorName(ELanguage curLanguage) override;

	virtual FString GetDescription(ELanguage curLanguage) override;

	virtual FString GetInteractionDescription(ELanguage curLanguage) override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	virtual void BeInteracted(APlayerCharacter* Sender) override;

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;

	virtual int GetProductCost() override;

	virtual FString GetProductName() override;

	virtual UTexture2D* GetProductImage() override;

/* item stats*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Stats")
	FString itemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Stats")
	FString itemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Stats")
	FString itemInteractionDesc;

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

