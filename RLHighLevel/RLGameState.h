// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RLGameState.generated.h"

class UFactoryEnvironment;

class APlayerRLController;
class APlayerCharacter;

class AEnvBoard;
class AEnvShop;
class AEnvSquare;

/*
* here is a good rule of thumb:
If you are changing the State of an actor, use a replicated variable. Use an OnRep callback if you want to respond to changes.
If you need a one-time Event, use a multicast.
*/

UCLASS()
class ARLGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ARLGameState();

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void BeginPlay();

    UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
    void debugFunctionOne(); // gamestate start creation
    UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
    void debugFunctionTwo(); // gamestate create board
    UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
    void debugFunctionThree(); // gamestate create shop
    UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
    void debugFunctionFour(); // gamestate close shop

protected:
    // players values
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        TArray<APlayerCharacter*> allPlayers;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
        FVector temporaryPlayerSpawnLocation = FVector(0.0f, 0.0f, 300.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
        int worldIndex;

public:
    /* all functions in gamestate (no getters) should be server which 
        spawn from gamemode and hold variables*/

    UFUNCTION(Server, Reliable, BlueprintCallable)
        void createPlayerBody(bool alive, int index);

    UFUNCTION(BlueprintCallable)
        APlayerCharacter* getPlayerBody(int controllerIndex);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Control")
        void playersReady();

protected:
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Map Manager")
    AEnvBoard* board = nullptr;
    
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Shop")
    AEnvShop* shop = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
    FVector boardLocation = FVector::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
    FRotator boardRotation = FRotator::ZeroRotator;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Stats")
    FVector shopLocation = FVector::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Stats")
    FRotator shopRotation = FRotator::ZeroRotator;

public:
    // netmulticast works, but it arise the error where client board can not access gamemode server
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "MapManager")
    void spawnBoard();
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "MapManager")
    void spawnShop();

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "MapManager")
    void initBoard();
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "MapManager")
    void initShop();

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "MapManager")
    void closeShop();

    UFUNCTION()
    TArray<APlayerCharacter*> getAllPlayers() const;

    AEnvBoard* getGameBoard() const { return board; }

    UFUNCTION(BlueprintCallable, Category = "Shop")
    AEnvShop* getShop() const { return shop; }

};