// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnum.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class EGameMode: uint8 {
	EInit,
	ELobby,
	EGameplay,
	EEnd
};