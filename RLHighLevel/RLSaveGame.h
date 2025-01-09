// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RLSaveGame.generated.h"

UCLASS(minimalapi)
class URLSaveGame: public USaveGame
{
	GENERATED_BODY()

public:
	URLSaveGame();

public:
	FString playerNameSaved;
};



