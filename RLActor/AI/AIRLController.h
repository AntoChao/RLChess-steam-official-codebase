#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "../../CommonEnum.h"

#include "AIRLController.generated.h"

class APiece;
class APlayerRLState;
class SimulatedBoardState;

UCLASS(BlueprintType, Blueprintable)
class AAIRLController : public AAIController
{
	GENERATED_BODY()

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player State")
    class APlayerRLState* rlPlayerState;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player State")
    class APlayerCharacter* rlPlayerBody;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Player State")
    int aiControllerIndex;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Player State")
    FColor AIColor;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Player State")
    bool isDied = false;

public:
	AAIRLController();

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Control")
    void setIndex(int controllerIndex);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Control")
    void gameStateCreateBody();

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Control")
    void serverPossesses(AAIRLController* currentController);

public:
    UFUNCTION(BlueprintCallable, Category = "Control")
    FString getPlayerName();
    UFUNCTION(BlueprintCallable, Category = "Control")
    void setAIPlayerColor(FColor aColor);
    UFUNCTION(BlueprintCallable, Category = "Control")
    FColor getPlayerColor();

    UFUNCTION(BlueprintCallable, Category = "Control")
    FString getCharacterName();

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
    class APlayerCharacter* controlledPlayer;

    virtual void OnPossess(APawn* InPawn) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
    USimulatedBoardState* aiBoardState;

    UFUNCTION(BlueprintCallable, Category = "Control")
    void synchronizeBoardState(TArray<APiece*> myPieces, TArray<APiece*> componentPieces);

    UFUNCTION(BlueprintCallable, Category = "Control")
    int32 Minimax(USimulatedBoardState* GameState, int32 Depth, int32 Alpha, int32 Beta, bool bIsAITurn, FMove& OutBestMove);

    FMove selectBestMove(TArray<APiece*> army);
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
    float findRaidus = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
    int DepthMiniMax = 3;
};