// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "../RLActor/Environment/EnvBoard.h"
#include "../RLActor/Player/PlayerCharacter.h"

#include "RoundManager.generated.h"

UCLASS(minimalapi)
class URoundManager : public UObject
{
	GENERATED_BODY()

public:
    // Access the singleton instance
    static URoundManager* get();

    // Ensure singleton is initialized
    static void initialize();

    // Ensure singleton is initialized
    void setAllPlayers(TArray<APlayerCharacter*> players);

	URoundManager();

    // The singleton instance
    static URoundManager* roundManagerInstance;

    // called by gamemode to initialize
    UFUNCTION(BlueprintCallable, Category = "RoundManager")
        void startRoundManagerSetUpRound();

    UFUNCTION(BlueprintCallable, Category = "RoundManager")
        void startRoundManagerGameplayRound();

    UFUNCTION(BlueprintCallable, Category = "RoundManager")
        void initialFreeTime();

    UFUNCTION(BlueprintCallable, Category = "RoundManager")
        void startTurns();

    UFUNCTION(BlueprintCallable, Category = "RoundManager")
        bool getIsSetUpTurn();

protected:

    // timerTime
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        bool isSetupTurn = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int turnTime = 10; // in segs

    // player and timers refences
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int numPlayers = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        TArray<APlayerCharacter*> allPlayers;

    // manage player round
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int roundCounter = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int turnCounter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int curPlayerIndex = 0;

    UFUNCTION(BlueprintCallable, Category = "RoundManager")
        int findInitPlayerIndex();

    UFUNCTION(BlueprintCallable, Category = "RoundManager")
        void checkIfGameEnd();

public:
    UFUNCTION(BlueprintCallable, Category = "RoundManager")
    void startNextPlayerTurn();
    UFUNCTION(BlueprintCallable, Category = "RoundManager")
    void endCurPlayerTurn();
};



