// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RoundManager.generated.h"

UCLASS(minimalapi)
class URoundManager : public UObject
{
	GENERATED_BODY()

public:
    // Access the singleton instance
    static URoundManager* get();

	URoundManager();

    // called by gamemode to initialize
    UFUNCTION(BlueprintCallable, Category = "RoundManager")
        void startRoundManager();

protected:
    // Ensure singleton is initialized
    static void initialize();

private:
    // The singleton instance
    static URoundManager* roundManagerInstance;

    // player and timers refences
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int numPlayers = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        TArray<APlayerCharacter*> allPlayers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        TArray<AEnvTimer*> allTimers; // timers index has to align with player


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
        void startNextPlayerTurn();
    UFUNCTION(BlueprintCallable, Category = "RoundManager")
        void endCurPlayerTurn();


    UFUNCTION(BlueprintCallable, Category = "RoundManager")
        void checkIfGameEnd();
};



