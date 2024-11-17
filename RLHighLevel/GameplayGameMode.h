// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RLChessGameMode.h"
#include "GameplayGameMode.generated.h"

class ARoundManager;

UCLASS(minimalapi)
class AGameplayGameMode : public ARLChessGameMode
{
	GENERATED_BODY()

public:
	AGameplayGameMode();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		ARoundManager* roundManager = nullptr;

	// all locations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		TArray<FLocation> playerLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		FLocation tableLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		TArray<FLocation> timerLocations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		TArray<FLocation> chairLocations;

	// all players
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void setupGame();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void createPlayers();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void createEnvironment();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void startRound();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void endGameplayGameMode();
};



