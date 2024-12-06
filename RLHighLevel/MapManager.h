// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "../RLActor/Environment/EnvBoard.h"
#include "../RLActor/Player/PlayerCharacter.h"
#include "../RLActor/Environment/EnvShop.h"
#include "MapManager.generated.h"

UCLASS(minimalapi)
class UMapManager : public UObject
{
    GENERATED_BODY()

public:
    static UMapManager* Instance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Manager")
    AEnvBoard* GameBoard;

    UFUNCTION(BlueprintCallable, Category = "Map Manager")
    void setGameMode(AGameplayGameMode* curGameMode);

    // gamemode, uobject doesnt have worldcontext
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Manager")
    AGameplayGameMode* gameplayGameMode = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Manager")
    TArray<AEnvSquare*> Buildings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    AEnvShop* shop = nullptr;

    UMapManager();

public:
    static UMapManager* get();
    static void initialize();

    void createMap();
    void createBoard(const TArray<APlayerCharacter*>& AllPlayers);
    void createBench(const TArray<APlayerCharacter*>& AllPlayers);

    void createShop();
    void closeShop();

    void createBuilding();  // Future implementation


public:
    AEnvBoard* getGameBoard() const { return GameBoard; }
    
    UFUNCTION(BlueprintCallable, Category = "Shop")
    AEnvShop* getShop() const { return shop; }

protected:
    void createWalls();

    TArray<FVector> CalculateWallPositions(const FVector& center, int rowSize, int wallHeight);
};