// Copyright Epic Games, Inc. All Rights Reserved.

#include "LobbyGameMode.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerStateClass = APlayerRLState::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    APlayerRLState* PlayerState = NewPlayer->PlayerState;
    if (PlayerState)
    {
        FString PlayerName = PlayerState->GetPlayerName();
        NotifyPlayers(FString::Printf(TEXT("%s has joined the lobby!"), *PlayerName));

        // Check if the lobby is full
        if (GameState && GameState->PlayerArray.Num() >= MaxPlayers)
        {
            NotifyPlayers(TEXT("The lobby is full."));
        }
    }
}

void ALobbyGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    APlayerRLState* PlayerState = Exiting->PlayerState;
    if (PlayerState)
    {
        FString PlayerName = PlayerState->GetPlayerName();
        NotifyPlayers(FString::Printf(TEXT("%s has left the lobby!"), *PlayerName));
    }
}

void ALobbyGameMode::updateReadyPlayer(AController* player) {
    APlayerRLState* PlayerState = Exiting->PlayerState;
    if (PlayerState)
    {
        if (playerState->isReady()) {
            readyPlayers++;
        }
        else {
            readyPlayers--;
        }
    }
}

void ALobbyGameMode::CheckStartGame()
{
    // Check if there are enough players to start
    if (GameState && GameState->PlayerArray.Num() == readyPlayers)
    {
        StartGame();
    }
    else
    {
        NotifyPlayers(TEXT("Waiting for more players to join..."));
    }
}

void ALobbyGameMode::StartGame()
{
    NotifyPlayers(TEXT("Starting the game!"));

    // Transition to gameplay level
    UWorld* World = GetWorld();
    if (World)
    {
        World->ServerTravel("/Game/Maps/GameplayMap?listen");
    }
}

void ALobbyGameMode::NotifyPlayers(const FString& Message)
{
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); ++Iterator)
    {
        APlayerController* PC = Iterator->Get();
        if (PC)
        {
            PC->ClientMessage(Message);
        }
    }
}
