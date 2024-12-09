#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "../../CommonEnum.h"

#include "AIRLController.generated.h"

class APiece;
class APlayerRLState;

UCLASS(BlueprintType, Blueprintable)
class AAIRLController : public AAIController
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player State")
    class APlayerRLState* rlPlayerState;

public:
	AAIRLController();

public:
    UFUNCTION(BlueprintCallable, Category = "Control")
    FString getPlayerName();
    UFUNCTION(BlueprintCallable, Category = "Control")
    FColor getPlayerColor();

    UFUNCTION(BlueprintCallable, Category = "Control")
    FString getCharacterName();

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
    class APlayerCharacter* controlledPlayer;

    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION(BlueprintCallable, Category = "AI Stats")
    void setCurrentValueMap();

    UFUNCTION(BlueprintCallable, Category = "AI Choice")
    void makeTheBestMove();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
    TMap<FVector2D, int32> valueMap;

    UFUNCTION(BlueprintCallable, Category = "AI Choice")
    FVector2D selectBestMove();

public:
    UFUNCTION(BlueprintCallable, Category = "AI Choice")
    void MoveToRandomLocation();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
    float findRaidus = 5000.0f;

};