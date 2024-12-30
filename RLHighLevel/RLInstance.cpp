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

void URLInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	if (Succeeded)
	{
		// GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
		UE_LOG(LogTemp, Warning, TEXT("Session created succeeed: %d"), Succeeded);
		GetWorld()->ServerTravel("/Game/HighLevel/GameplayLevel?listen");
	}
}

void URLInstance::OnFindSessionComplete(bool Succeeded)
{
	if (Succeeded)
	{
		UE_LOG(LogTemp, Warning, TEXT("On Find Session succeeed: %d"), Succeeded);
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

		if (SearchResults.Num())
		{
			SessionInterface->JoinSession(0, FName("Crete Session"), SearchResults[0]);
		}
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

void URLInstance::CreateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;

	SessionInterface->CreateSession(0, FName("Crete Session"), SessionSettings);
}

void URLInstance::JoinServer()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}


EGameModeEnum URLInstance::getCurGameMode() {
	return curGameMode;
}

ELanguage URLInstance::getLanguage()
{
	return gameLanguage;
}