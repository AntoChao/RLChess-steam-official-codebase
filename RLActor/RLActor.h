// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DrawDebugHelpers.h"
#include "../CommonEnum.h"
#include "RLActor.generated.h"

class APlayerCharacter;

UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class URLActor : public UInterface
{
	GENERATED_BODY()
};

class IRLActor
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    virtual FString GetActorName(ELanguage curLanguage) = 0;

    UFUNCTION(BlueprintCallable)
    virtual FString GetDescription(ELanguage curLanguage) = 0;

    UFUNCTION(BlueprintCallable)
    virtual FString GetInteractionDescription(ELanguage curLanguage) = 0;

    UFUNCTION(BlueprintCallable)
    virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) = 0;

    UFUNCTION(BlueprintCallable)
    virtual void BeInteracted(APlayerCharacter* Sender) = 0;

    UFUNCTION(BlueprintCallable)
    virtual void BeUnInteracted(APlayerCharacter* Sender) = 0;
};