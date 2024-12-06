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
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
        bool isReady;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
        FString playerName;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
        FColor playerColor = FColor::Yellow;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
        FString characterName = TEXT("testing");

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

