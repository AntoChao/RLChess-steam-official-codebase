// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RLActor.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class URLActor : public UInterface
{
	GENERATED_BODY()
};

class IRLActor
{
    GENERATED_BODY()

public:
    virtual FName GetActorName() const = 0;

    virtual FText GetDescription() const = 0;

    virtual bool IsAbleToInteract(RLPlayer* Sender) const = 0;

    virtual void BeInteracted(RLPlayer* Sender) = 0;

    virtual void BeUnInteracted(RLPlayer* Sender) = 0;
};