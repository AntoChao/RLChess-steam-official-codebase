// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RLActor.generated.h"

class APlayerCharacter;

UINTERFACE(MinimalAPI, Blueprintable)
class URLActor : public UInterface
{
	GENERATED_BODY()
};

class IRLActor
{
    GENERATED_BODY()

public:
    virtual FString GetActorName() = 0;

    virtual FString GetDescription() = 0;

    virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) = 0;

    virtual void BeInteracted(APlayerCharacter* Sender) = 0;

    virtual void BeUnInteracted(APlayerCharacter* Sender) = 0;
};