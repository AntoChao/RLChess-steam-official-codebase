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

class UHUDLobby;

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
	UPROPERTY(BlueprintReadWrite, Category = "Level Control")
		int sessionSize = 2;

// online sessions steam
	TSharedPtr<FOnlineSessionSearch> searchSettings ;

	IOnlineSessionPtr SessionInterface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
	bool foundSuccessed = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
	TArray<FSessionInfo> SessionList;

	UPROPERTY(BlueprintReadWrite, Category = "Level Control")
	bool testing = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "Level Control")
	FName curSessionName;

	virtual void Init() override;
	virtual void Shutdown() override;

	void createSaveFile();
	FString generateRandomUserString();
	void saveGame();
	void loadGame();
	
	/* saved data*/
	UPROPERTY(BlueprintReadWrite, Category = "Level Control")
	FString curPlayerName;

	UFUNCTION(BlueprintCallable)
	FString getName();

	/* multiplayer actions*/
	UFUNCTION(BlueprintCallable)
	void hostSession(FName sessionName, int numPlayers);
	UFUNCTION(BlueprintCallable)
	void hostSessionCompleted(FName sessionName, bool createdSession);

	UFUNCTION(BlueprintCallable)
	void searchForSessions();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
	UHUDLobby* searchWidget;
	UFUNCTION(BlueprintCallable)
	void setSearchWidget(UHUDLobby* aSearchWidget);
	UFUNCTION(BlueprintCallable)
	void searchForSessionsCompleted(bool searchCompleted);

	UFUNCTION(BlueprintCallable)
	void joinSession(int32 SessionIndex);

	void joinSessionCompleted(FName sessionName, EOnJoinSessionCompleteResult::Type joinResult);
	// void joinSessionCompleted(FName sessionName, EOnJoinSessionCompleteResult joinResult);
	
	UFUNCTION(BlueprintCallable)
	bool travelToSession(FName sessionName);

	UFUNCTION(BlueprintCallable)
	void endSession();
	UFUNCTION(BlueprintCallable)
	void travelBackToLobby();

	UFUNCTION(BlueprintCallable)
	void endSessionCompleted(FName sessionName, bool endedSession);

	UFUNCTION(BlueprintCallable)
	void destroySession(FName sessionName);
	UFUNCTION(BlueprintCallable)
	void destroySessionCompleted(FName sessionName, bool destroyedSession);

	FOnCreateSessionCompleteDelegate createSessionsCompletedDelegate;
	FDelegateHandle createSessionsCompletedHandle;
	FOnFindSessionsCompleteDelegate searchForSessionsCompletedDelegate;
	FDelegateHandle searchForSessionsCompletedHandle;
	FOnJoinSessionCompleteDelegate joinSessionCompletedDelegate;
	FDelegateHandle joinSessionCompletedHandle;
	FOnEndSessionCompleteDelegate endSessionCompletedDelegate;
	FDelegateHandle endSessionCompletedHandle;
	FOnDestroySessionCompleteDelegate destroySessionCompletedDelegate;
	FDelegateHandle destroySessionCompletedHandle;

	UPROPERTY(BlueprintReadWrite, Category = "Level Control")
	bool sessionAlreadyEnded;

public:
	UFUNCTION(BlueprintCallable, Category = "Level Control")
		EGameModeEnum getCurGameMode();

	UFUNCTION(BlueprintCallable, Category = "Level Control")
		void setLanguage(ELanguage aLanguage);

	UFUNCTION(BlueprintCallable, Category = "Level Control")
		ELanguage getLanguage();
};



