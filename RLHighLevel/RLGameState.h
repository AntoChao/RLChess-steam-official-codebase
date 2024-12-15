// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RLGameState.generated.h"

class UFactoryPlayer;
class UFactoryEnvironment;
class UFactoryItem;
class UFactoryPiece;

class APlayerRLController;
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
    UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
        void setUpGameState();

    UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "GameMode")
        void setupSingletonClasses();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
        FVector temporaryPlayerSpawnLocation = FVector(0.0f, 0.0f, 300.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
        TSubclassOf<UFactoryPlayer> playerFactoryClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
        TSubclassOf<UFactoryEnvironment> envFactoryClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
        TSubclassOf<UFactoryItem> itemFactoryClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
        TSubclassOf<UFactoryPiece> pieceFactoryClass;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
        UFactoryPlayer* playerFactoryInstance = nullptr;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
        UFactoryEnvironment* environmentFactoryInstance = nullptr;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
        UFactoryItem* itemFactoryInstance = nullptr;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
        UFactoryPiece* pieceFactoryInstance = nullptr;

protected:
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        TArray<APlayerCharacter*> allPlayers;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
        int curPlayerIndex;
public:
    UFUNCTION(Server, Reliable, BlueprintCallable)
        void createPlayerBody();

    UFUNCTION(BlueprintCallable)
        APlayerCharacter* getPlayerBody();
};