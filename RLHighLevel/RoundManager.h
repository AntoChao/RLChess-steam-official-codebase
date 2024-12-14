// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "RoundManager.generated.h"

class AEnvBoard;
class APlayerCharacter;
class AEnvShop;
class APiece;

UCLASS(minimalapi)
class URoundManager : public UObject
{
	GENERATED_BODY()

public:
    URoundManager();

    // Access the singleton instance
    static URoundManager* get();

    // Ensure singleton is initialized
    static void initialize();

    // The singleton instance
    static URoundManager* roundManagerInstance;

    UFUNCTION(BlueprintCallable, Category = "RoundManager")
    void setGameMode(AGameplayGameMode* curGameMode);

    // Ensure singleton is initialized
    void setAllPlayers(TArray<APlayerCharacter*> players);

    void addPlayers(APlayerCharacter* aPlayer);

    TArray<APlayerCharacter*> getAllPlayers();

    // called by gamemode to initialize
    UFUNCTION(Server, Reliable, Category = "RoundManager")
        void startRoundManagerSetUpRound();

    UFUNCTION(Server, Reliable, Category = "RoundManager")
        void startRoundManagerGameplayRound();


    UFUNCTION(BlueprintCallable, Category = "GameMode")
        void setMapColor();
    UFUNCTION(BlueprintCallable, Category = "GameMode")
        void setPlayerBench();

    UFUNCTION(Server, Reliable, Category = "RoundManager")
        void setPlayerInitLocation();

    UFUNCTION(Server, Reliable, Category = "RoundManager")
        void spawnShop();

    UFUNCTION(Server, Reliable, Category = "RoundManager")
        void startPlayerSetUpTime();
    UFUNCTION(Server, Reliable, Category = "RoundManager")
        void endPlayerSetUpTime();

    UFUNCTION(Server, Reliable, Category = "RoundManager")
        void startPlayerPreparePhase();

    UFUNCTION(Server, Reliable, Category = "RoundManager")
        void startPieceMovingPhase();

    UFUNCTION(BlueprintCallable, Category = "RoundManager")
        bool getIsPlayerSetUpTime();

    UFUNCTION(BlueprintCallable, Category = "RoundManager")
        bool checkIfGameEnd();
    UFUNCTION(Server, Reliable, Category = "RoundManager")
        void gameEnd();

protected:
    // gamemode, uobject doesnt have worldcontext
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    AGameplayGameMode* gameplayGameMode = nullptr;

    // timerTime
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        bool isPlayerSetupTime = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        FTimerHandle playerSetUpTimerHandle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int playerSetUpTimerSegs = 10;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        FTimerHandle playerTurnTimerHandle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int turnTimeInSegs = 10; // in segs

    // player and timers refences
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int numPlayers = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        TArray<APlayerCharacter*> allPlayers;

    // manage player round
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int roundCounter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        APlayerCharacter* winner = nullptr;

    UPROPERTY(BlueprintReadWrite, Category = "Round Manager")
        FTimerHandle playerPrepareTimerHandle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int playerPrepareTimerSegs = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        FTimerHandle piecesMovedTimerHandle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int piecesMovedTimerSegs = 5;
};



