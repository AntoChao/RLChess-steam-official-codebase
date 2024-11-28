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

UENUM(BlueprintType, Blueprintable)
enum class EInteraction : uint8 {
	EUseItem,
	EPickUpItem,
	EShop,
	ESelectPiece,
	EPlacePiece,
	ENone
};

UENUM(BlueprintType, Blueprintable)
enum class EPieceStatus : uint8 {
	EInShop,
	EInBench,
	EInBoard,
	ENone
};

UENUM(BlueprintType, Blueprintable)
enum class EPieceDirection : uint8 {
	EUp,
	EDown,
	ELeft,
	ERight,
	ENone
};