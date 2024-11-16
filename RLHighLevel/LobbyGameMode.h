// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RLChessGameMode.h"
#include "LobbyGameMode.generated.h"

UCLASS(minimalapi)
class ALobbyGameMode : public ARLChessGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

    // Called when a player joins the game
    virtual void PostLogin(APlayerController* NewPlayer) override;

    // Called when a player leaves the game
    virtual void Logout(AController* Exiting) override;

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void updateReadyPlayer(AController* player);

    // Check if the game can start
    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void CheckStartGame();

protected:
    // Maximum number of players
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lobby")
    const int32 MaxPlayers = 4;

    // Minimum number of players to start the game
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lobby")
    const int32 MinPlayersToStart = 2;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lobby")
    const int32 readyPlayers = 0;

    // Start the game
    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void StartGame();

    // Notify all players about an event
    void NotifyPlayers(const FString& Message);
};



