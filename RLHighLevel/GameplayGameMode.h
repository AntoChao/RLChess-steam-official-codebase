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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		int numPlayers = 0;

	// all locations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		FLocation tableLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		TArray<FLocation> timerLocations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		TArray<FLocation> chairLocations;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void setupGame();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void createPlayers();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void createMap();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void startRound();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void updateGameplayGameState();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void checkIsAnyWinner();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void endGameplayGameMode();
};



