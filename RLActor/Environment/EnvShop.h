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

	virtual FString GetActorName() override;

	virtual FString GetDescription() override;

	virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

	virtual void BeInteracted(APlayerCharacter* Sender) override;

	virtual void BeUnInteracted(APlayerCharacter* Sender) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void createRandomShop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Stats")
	int piecesProductTotalNum = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Stats")
	TArray<APiece*> piecesInShop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Widget")
	TSubclassOf<UShopWidget> shopWidgetClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Widget")
	UShopWidget* shopHUD = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void showShopHUDToPlayer(APlayerCharacter* player);

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void sellProduct(APlayerCharacter* player, APiece* specificPiece);

	UFUNCTION(BlueprintCallable, Category = "Square Stats")
	void refreshShop(APlayerCharacter* player);
};

