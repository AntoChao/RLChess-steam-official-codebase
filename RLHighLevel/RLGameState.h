// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RLGameState.generated.h"

class APlayerCharacter;

UCLASS()
class ARLGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ARLGameState();

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void BeginPlay();

protected:
    UFUNCTION(BlueprintCallable)
        void setUpGameState();

protected:
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        TArray<APlayerCharacter*> allPlayers;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int playerBodyIndex;

public
    UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
        void addPlayerBody(APlayerCharacter* aBody);

    UFUNCTION(Server, Reliable, BlueprintCallable)
        APlayerCharacter* getCorrespondPlayerBody();
};