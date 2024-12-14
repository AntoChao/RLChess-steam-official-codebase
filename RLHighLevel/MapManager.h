// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapManager.generated.h"

class AEnvBoard;
class APlayerCharacter;
class AEnvShop;
class AEnvSquare;

UCLASS(minimalapi)
class UMapManager : public UObject
{
    GENERATED_BODY()

public:
    static UMapManager* Instance;

    UMapManager();

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Map Manager")
    AEnvBoard* GameBoard;

    UFUNCTION(BlueprintCallable, Category = "Map Manager")
    void setGameMode(AGameplayGameMode* curGameMode);

    // gamemode, uobject doesnt have worldcontext
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Map Manager")
    AGameplayGameMode* gameplayGameMode = nullptr;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Shop")
    AEnvShop* shop = nullptr;

public:
    static UMapManager* get();
    static void initialize();

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "MapManager")
    void createMap();
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "MapManager")
    void createBoard();
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "MapManager")
    void createBench(const TArray<APlayerCharacter*>& AllPlayers);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "MapManager")
    void createShop();
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "MapManager")
    void closeShop();

    UFUNCTION(BlueprintCallable, Category = "MapManager")
    void createBuilding();  // Future implementation


public:
    AEnvBoard* getGameBoard() const { return GameBoard; }
    
    UFUNCTION(BlueprintCallable, Category = "Shop")
    AEnvShop* getShop() const { return shop; }

};