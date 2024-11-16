// Copyright Epic Games, Inc. All Rights Reserved.

#include "RLChessGameMode.h"

#include "../RLActor/Player/PlayerRLState.h"

ARLChessGameMode::ARLChessGameMode()
{
	PlayerStateClass = APlayerRLState::StaticClass();
}
