// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RLChessGameMode.h"
#include "RoundManager.h"
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
		URoundManager* roundManager = nullptr;

	// all locations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		TArray<FVector> playerLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		FVector tableLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		TArray<FVector> timerLocations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		TArray<FVector> chairLocations;

	// all players
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void setupGame();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void createPlayers();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void createEnvironment();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void startRoundSetUp();
		UFUNCTION(BlueprintCallable, Category = "GameMode")
		void startRoundGameplay();

public:
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void endGameplayGameMode(APlayerCharacter* winner);
};



