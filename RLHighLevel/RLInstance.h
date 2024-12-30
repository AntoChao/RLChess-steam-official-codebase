// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Engine/GameInstance.h"

#include "../CommonEnum.h"

#include "RLInstance.generated.h"

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

	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnFindSessionComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
	void CreateServer();

	UFUNCTION(BlueprintCallable)
	void JoinServer();


public:
	UFUNCTION(BlueprintCallable, Category = "Level Control")
		EGameModeEnum getCurGameMode();

	UFUNCTION(BlueprintCallable, Category = "Level Control")
		ELanguage getLanguage();
};



