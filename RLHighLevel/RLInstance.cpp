// Copyright Epic Games, Inc. All Rights Reserved.

#include "RLInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

URLInstance::URLInstance(){
	// curGameMode = EGameModeEnum::EGameplay;
}

void URLInstance::openNextLevel(EGameModeEnum gameToOpen)
{
	curGameMode = gameToOpen;
	switch (gameToOpen)
	{
		case EGameModeEnum::EInit:
		{
			UGameplayStatics::OpenLevel(this, FName("InitLevel"));
			break;
		}
		case EGameModeEnum::ELobby:
		{
			UGameplayStatics::OpenLevel(this, FName("LobbyLevel"));
			break;
		}
		case EGameModeEnum::EGameplay:
		{
			UGameplayStatics::OpenLevel(this, FName("GameplayLevel"));
			break;
		}
		case EGameModeEnum::EEnd:
		{
			UGameplayStatics::OpenLevel(this, FName("EndLevel"));
			break;
		}
		default:
			UGameplayStatics::OpenLevel(this, FName("InitLevel"));
			break;
	}
}

// online sessions steam

void URLInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &URLInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &URLInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &URLInstance::OnJoinSessionComplete);
		}
	}
}

void URLInstance::CreateServer(FName sessionName, int numPlayers)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = numPlayers;

	// SessionInterface->CreateSession(0, sessionName, SessionSettings);
	SessionInterface->CreateSession(0, sessionName, SessionSettings);
}

void URLInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	if (Succeeded)
	{
		// GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
		UE_LOG(LogTemp, Warning, TEXT("Session created succeeed: %d"), Succeeded);
		if (!testing)
		{
			GetWorld()->ServerTravel("/Game/HighLevel/GameplayLevel?listen");
		}
	}
}

void URLInstance::SearchServer()
{
	foundSuccessed = false;

	UE_LOG(LogTemp, Warning, TEXT("Server searching"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void URLInstance::OnFindSessionComplete(bool Succeeded)
{
	
	foundSuccessed = Succeeded;
	if (Succeeded && SessionSearch.IsValid())
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

		UE_LOG(LogTemp, Warning, TEXT("On Find Session succeeded: %d"), Succeeded);
		// TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults; // Assuming these are now by value
		UE_LOG(LogTemp, Warning, TEXT("Session result found num: %d"), SearchResults.Num());

		if (SearchResults.Num() > 0)
		{
			SessionList.Empty();
			for (int32 i = 0; i < SearchResults.Num(); ++i)
			{
				FOnlineSessionSearchResult& SearchResult = SearchResults[i];
				FSessionInfo Info;
				Info.SessionIndex = i;
				Info.SessionName = SearchResult.GetSessionIdStr();
				Info.CurrentPlayers = SearchResult.Session.SessionSettings.NumPublicConnections -
					SearchResult.Session.NumOpenPublicConnections;
				Info.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
				SessionList.Add(Info);
				
				UE_LOG(LogTemp, Warning, TEXT("Session search info created"));
			}
		}
	}
}

void URLInstance::JoinServer(int32 SessionIndex)
{
	if (SessionInterface.IsValid() && SessionList.IsValidIndex(SessionIndex))
	{
		const FSessionInfo& SelectedSession = SessionList[SessionIndex];
		FName SessionName(*SelectedSession.SessionName);
		FOnlineSessionSearchResult SearchResult = SessionSearch->SearchResults[SessionIndex];

		SessionInterface->JoinSession(0, SessionName, SearchResult);
	}
}

void URLInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("Session JOIN completed"));
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}


EGameModeEnum URLInstance::getCurGameMode() {
	return curGameMode;
}

ELanguage URLInstance::getLanguage()
{
	return gameLanguage;
}