// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../RLActor.h"
#include "UObject/Class.h"
#include "ShopWidget.h"
#include "EnvShop.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AEnvShop : public AActor, public IRLActor
{
	GENERATED_BODY()

public:
	AEnvShop();
	
	virtual void BeginPlay() override;

	virtual FString GetActorName() override;

	virtual FString GetDescription() override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	virtual void BeInteracted(APlayerCharacter* Sender) override;

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* shopMesh;

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void createRandomShop();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Stats")
	int piecesProductTotalNum = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Stats")
	TArray<TScriptInterface<IRLProduct>> productsInShop;

public:
	UFUNCTION(BlueprintCallable, Category = "Shop Stats")
	void sellProduct(APlayerCharacter* player, TScriptInterface<IRLProduct> specificProduct);

	UFUNCTION(BlueprintCallable, Category = "Shop Stats")
	void refreshShop(APlayerCharacter* player);

	UFUNCTION(BlueprintCallable, Category = "Shop Stats")
	void closeShop();

};

