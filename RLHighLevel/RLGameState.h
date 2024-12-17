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
        void createPlayerBody();

    UFUNCTION(BlueprintCallable)
        APlayerCharacter* getPlayerBody(int controllerIndex);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Control")
        void playersReady();

protected:
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Map Manager")
    AEnvBoard* board = nullptr;
    
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Shop")
    AEnvShop* shop = nullptr;

public:
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "MapManager")
    void setBoard();

    UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "MapManager")
    void setShop();
    UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "MapManager")
    void closeShop(); //server not working

    UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "MapManager")
    void createBench();

    AEnvBoard* getGameBoard() const { return board; }

    UFUNCTION(BlueprintCallable, Category = "Shop")
    AEnvShop* getShop() const { return shop; }

};