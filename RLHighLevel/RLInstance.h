// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UObject/NoExportTypes.h"

#include "Engine/GameInstance.h"

#include "../CommonEnum.h"

#include "RLInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Session Info")
	int32 SessionIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Session Info")
	FString SessionName;

	UPROPERTY(BlueprintReadOnly, Category = "Session Info")
	int32 CurrentPlayers;

	UPROPERTY(BlueprintReadOnly, Category = "Session Info")
	int32 MaxPlayers;
};

UCLASS(minimalapi)
class URLInstance: public UGameInstance
{
	GENERATED_BODY()

public:
	URLInstance();

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Level Control")
		ELanguage gameLanguage = ELanguage::EEnglish;

	UPROPERTY(BlueprintReadWrite, Category = "Level Control")
		EGameModeEnum curGameMode;

public:
	UFUNCTION(BlueprintCallable, Category = "Level Control")
		void openNextLevel(EGameModeEnum gameToOpen);

// online sessions steam
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	IOnlineSessionPtr SessionInterface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
	bool foundSuccessed = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
	TArray<FSessionInfo> SessionList;

	UPROPERTY(BlueprintReadWrite, Category = "Level Control")
	bool testing = false;

	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnFindSessionComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
	void CreateServer();

	UFUNCTION(BlueprintCallable)
	void JoinServerSimple();

	/*
	UFUNCTION(BlueprintCallable)
	void CreateServer(FName sessionName, int numPlayers);

	UFUNCTION(BlueprintCallable)
	void SearchServer();

	UFUNCTION(BlueprintCallable)
	void JoinServer(int32 SessionIndex);*/

public:
	UFUNCTION(BlueprintCallable, Category = "Level Control")
		EGameModeEnum getCurGameMode();

	UFUNCTION(BlueprintCallable, Category = "Level Control")
		ELanguage getLanguage();
};



