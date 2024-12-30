// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../RLActor.h"
#include "../RLProduct.h"

#include "EnvShop.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AEnvShop : public AActor, public IRLActor
{
	GENERATED_BODY()

public:
	AEnvShop();
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual FString GetActorName(ELanguage curLanguage) override;

	virtual FString GetDescription(ELanguage curLanguage) override;

	virtual FString GetInteractionDescription(ELanguage curLanguage) override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	virtual void BeInteracted(APlayerCharacter* Sender) override;

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
	void debugFunction();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Square Stats")
	void createRandomShop();

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Stats")
	int piecesProductTotalNum = 25;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Shop Stats")
	TArray<TScriptInterface<IRLProduct>> productsInShop;

public:
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shop Stats")
	void sellProduct(APlayerCharacter* player, APiece* specificProduct);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shop Stats")
	void refillProduct(APiece* specificProduct);


	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shop Stats")
	void refreshShop(APlayerCharacter* player);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shop Stats")
	void closeShop();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shop Stats")
	void fullFill(APlayerCharacter* controlledPlayer);

	UFUNCTION(BlueprintCallable, Category = "Shop Stats")
	APiece* selectRandomProduct();

};

