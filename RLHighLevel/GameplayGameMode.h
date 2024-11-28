// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RLChessGameMode.h"
#include "RoundManager.h"

#include "../RLActor/Factory/FactoryEnvironment.h"
#include "../RLActor/Factory/FactoryItem.h"
#include "../RLActor/Factory/FactoryPiece.h"
#include "../RLActor/Factory/FactoryPlayer.h"

#include "GameplayGameMode.generated.h"

class ARoundManager;
class UFactoryPlayer;
class UFactoryEnvironment;
class UFactoryItem;
class UFactoryPiece;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		AEnvBoard* gameBoard = nullptr;

	// all factory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
		TSubclassOf<UFactoryPlayer> playerFactoryClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
		TSubclassOf<UFactoryEnvironment> envFactoryClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
		TSubclassOf<UFactoryItem> itemFactoryClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
		TSubclassOf<UFactoryPiece> pieceFactoryClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
	UFactoryPlayer* playerFactoryInstance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
	UFactoryEnvironment* environmentFactoryInstance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
	UFactoryItem* itemFactoryInstance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
	UFactoryPiece* pieceFactoryInstance = nullptr;

protected:
	// all locations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		FVector tableLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		FVector boardLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		TArray<FVector> timerLocations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		TArray<FVector> chairLocations;

	// set up
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void setupGame();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void setupSingletonClasses();
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

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	AEnvBoard* getBoard();
};



