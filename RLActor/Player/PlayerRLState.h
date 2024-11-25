#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerRLState.generated.h"

UCLASS(config=Game)
class APlayerRLState : public APlayerState
{
	GENERATED_BODY()

public:
    // Properties
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player Stats")
        bool isReady;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player Stats")
        FString playerName;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player Stats")
        FString characterName;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player Stats")
        TArray<FString> playerPiecesName;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player Stats")
        FColor playerColor;
    
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

