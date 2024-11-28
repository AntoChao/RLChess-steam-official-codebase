// Copyright Epic Games, Inc. All Rights Reserved.

#include "RLInstance.h"

URLInstance::URLInstance(){
	curGameMode = EGameMode::EGameplay;
}

void URLInstance::openNextLevel(EGameMode gameToOpen)
{
	curGameMode = gameToOpen;
	switch (gameToOpen)
	{
		case EGameMode::EInit:
		{
			UGameplayStatics::OpenLevel(this, FName("InitLevel"));
			break;
		}
		case EGameMode::ELobby:
		{
			UGameplayStatics::OpenLevel(this, FName("LobbyLevel"));
			break;
		}
		case EGameMode::EGameplay:
		{
			UGameplayStatics::OpenLevel(this, FName("GameplayLevel"));
			break;
		}
		case EGameMode::EEnd:
		{
			UGameplayStatics::OpenLevel(this, FName("EndLevel"));
			break;
		}
		default:
			UGameplayStatics::OpenLevel(this, FName("InitLevel"));
			break;
	}
}

EGameMode URLInstance::getCurGameMode() {
	return curGameMode;
}