// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RLChessGameMode.h"
#include "MapManager.h"
#include "RoundManager.h"

#include "../RLActor/Factory/FactoryEnvironment.h"
#include "../RLActor/Factory/FactoryItem.h"
#include "../RLActor/Factory/FactoryPiece.h"
#include "../RLActor/Factory/FactoryPlayer.h"

#include "GameplayGameMode.generated.h"

class URoundManager;
class UMapManager;
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
		UMapManager* mapManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
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

	// all locations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
	float playerSpawnHeight = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table Stats")
		FVector tableLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table Stats")
		FRotator tableRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
		FVector boardLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
		FRotator boardRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
		int floorSize = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Stats")
		FVector shopLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Stats")
		FRotator shopRotation = FRotator::ZeroRotator;

protected:
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
};



