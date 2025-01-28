#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "../../RLActor.h"
#include "../../../CommonEnum.h"

#include "HUDGameplay.generated.h"

UCLASS()
class UHUDGameplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Level Control")
		ELanguage hudLanguage = ELanguage::EEnglish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection Control")
		bool isWaitingTime = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection Control")
		bool isSetupTime = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection Control")
		int ownerMoney;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
		bool isAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		bool isPlayerTurn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection Control")
		TScriptInterface<IRLActor> curDetectedActor;

protected:
	UFUNCTION(BlueprintCallable, Category = "Control")
	void updateSetupTimeDef();
	UFUNCTION(BlueprintCallable, Category = "Control")
	void updatePlayerTurnDef();
	UFUNCTION(BlueprintCallable, Category = "Control")
	void updatePieceMovingDef();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString isSetupTimeDef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString isPlayerTurnDef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString pieceMovingDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString isSetupTimeEng;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString isSetupTimeChi;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString isSetupTimeSpa;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString isPlayerTurnEng;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString isPlayerTurnChi;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString isPlayerTurnSpa;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString pieceMovingEng;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString pieceMovingChi;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
	FString pieceMovingSpa;
};

