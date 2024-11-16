// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance"
#include "RLInstance.generated.h"

UCLASS(minimalapi)
class URLInstance: public UGameInstance
{
	GENERATED_BODY()

public:
	URLInstance();

private:
	UPROPERTY(BlueprintReadWrite, Category = "Level Control")
		EGameMode curGameMode;

public:
	UFUNCTION(BlueprintCallable, Category = "Level Control")
		void openNextLevel(EGameMode gameToOpen);

	UFUNCTION(BlueprintCallable, Category = "Level Control")
		EGameMode getCurGameMode();
};



