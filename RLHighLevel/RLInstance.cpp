// Copyright Epic Games, Inc. All Rights Reserved.

#include "RLInstance.h"

#include "../RLActor/Player/RLWidget/HUDLobby.h"
#include "Kismet/GameplayStatics.h"
#include "RLSaveGame.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

URLInstance::URLInstance()
{
	createSessionsCompletedDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &URLInstance::hostSessionCompleted);
	searchForSessionsCompletedDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &URLInstance::searchForSessionsCompleted);
	joinSessionCompletedDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &URLInstance::joinSessionCompleted);
	endSessionCompletedDelegate = FOnEndSessionCompleteDelegate::CreateUObject(this, &URLInstance::endSessionCompleted);
	destroySessionCompletedDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &URLInstance::destroySessionCompleted);
	
}

// online sessions steam

void URLInstance::Init()
{
	Super::Init();
	loadGame();
}

void URLInstance::Shutdown()
{
	Super::Shutdown();
	saveGame();
}

void URLInstance::hostSession(FName sessionName, int numPlayers)
{
	if (IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr onlineSessionInterface = onlineSubsystem->GetSessionInterface())
		{
			sessionAlreadyEnded = false;
			curSessionName = sessionName;

			UE_LOG(LogTemp, Error, TEXT("HOST SESSION CHECK USER NAME: %s"), *curPlayerName);
			UE_LOG(LogTemp, Error, TEXT("HOST SESSION NAME: %s"), *curSessionName.ToString());

			createSessionsCompletedHandle = onlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(createSessionsCompletedDelegate);

			UE_LOG(LogTemp, Warning, TEXT("CreateServer"));

			FOnlineSessionSettings sessionSettings;

			sessionSettings.bIsDedicated = false;
			sessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
			sessionSettings.bShouldAdvertise = true;

			sessionSettings.NumPublicConnections = numPlayers;  // Total players including the host
			sessionSize = numPlayers + 1;
			// sessionSettings.NumPrivateConnections = 0;

			sessionSettings.bUsesPresence = true;
			sessionSettings.bAllowJoinInProgress = true;
			sessionSettings.bAllowInvites = true;
			sessionSettings.bAllowJoinViaPresence = true;
			sessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
			sessionSettings.bUseLobbiesIfAvailable = true;

			sessionSettings.Set(FName("SESSION_NAME"), sessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService); // Storing session name

			const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			// if (onlineSessionInterface->CreateSession(0, sessionName, sessionSettings)) //*sessionSettings
			if (onlineSessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), sessionName, sessionSettings)) //*sessionSettings
			{
				UE_LOG(LogTemp, Error, TEXT("A session has been created"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Session Interface has failed to be created"));

				endSession();
			}
		}
	}
}

void URLInstance::hostSessionCompleted(FName sessionName, bool createdSession)
{
	if (IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr onlineSessionInterface = onlineSubsystem->GetSessionInterface())
		{
			if (createdSession)
			{
				onlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(createSessionsCompletedHandle);

				// Log for successful session creation
				UE_LOG(LogTemp, Error, TEXT("Session %d created successfully."), createdSession);

				// have to put the level into listen for listen erver
				GetWorld()->ServerTravel("/Game/HighLevel/GameplayLevel?listen");
				onlineSessionInterface->StartSession(sessionName);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create session."));
			}
		}
	}
}

void URLInstance::setSearchWidget(UHUDLobby* aSearchWidget)
{
	searchWidget = aSearchWidget;
}

void URLInstance::searchForSessions()
{
	if (IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr onlineSessionInterface = onlineSubsystem->GetSessionInterface())
		{
			searchForSessionsCompletedHandle = onlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(searchForSessionsCompletedDelegate);

			searchSettings = MakeShareable(new FOnlineSessionSearch());
			searchSettings->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
			searchSettings->MaxSearchResults = 10000;
			searchSettings->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

			// searchSettings->PingBucketSize = 60;
			// searchSettings->TimeoutInSeconds = 10.0f;

			const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			// if (onlineSessionInterface->FindSessions(0, searchSettings.ToSharedRef()))

			if (onlineSessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), searchSettings.ToSharedRef()))
			{
				UE_LOG(LogTemp, Error, TEXT("search started"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("search unable to start"));
			}
		}
	}
}

void URLInstance::searchForSessionsCompleted(bool Succeeded)
{
	foundSuccessed = Succeeded;

	if (IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr onlineSessionInterface = onlineSubsystem->GetSessionInterface())
		{
			// clear the handle and stop listening for the completion of the search operation
			onlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(searchForSessionsCompletedHandle);

			UE_LOG(LogTemp, Error, TEXT("search found %d sessions after completing search."), searchSettings->SearchResults.Num());

			TArray<FOnlineSessionSearchResult> SearchResults = searchSettings->SearchResults;

			if (SearchResults.Num() > 0)
			{
				SessionList.Empty();
				for (int32 i = 0; i < SearchResults.Num(); ++i)
				{
					FOnlineSessionSearchResult& SearchResult = SearchResults[i];
					FSessionInfo Info;
					FString SessionDisplayName;
					SearchResult.Session.SessionSettings.Get(FName("SESSION_NAME"), SessionDisplayName);  // Retrieving the session name

					Info.SessionIndex = i;
					Info.SessionName = !SessionDisplayName.IsEmpty() ? SessionDisplayName : TEXT("Default Server");  // Using a default if no name is found
					Info.CurrentPlayers = SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections;
					Info.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;

					SessionList.Add(Info);

					UE_LOG(LogTemp, Warning, TEXT("Session search info created for %s"), *Info.SessionName);
				}

				if (searchWidget)
				{
					searchWidget->refreshSearchResult(SessionList);
				}
			}
		}
	}
}

void URLInstance::joinSession(int32 SessionIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("JoinSession being called"));
	UE_LOG(LogTemp, Error, TEXT("Session index found: %d."), SessionIndex);
	UE_LOG(LogTemp, Error, TEXT("Session list length: %d."), SessionList.Num());

	if (IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr onlineSessionInterface = onlineSubsystem->GetSessionInterface())
		{
			// if (SessionInterface.IsValid() && SessionList.IsValidIndex(SessionIndex))
			if (SessionList.IsValidIndex(SessionIndex))
			{
				UE_LOG(LogTemp, Warning, TEXT("Joining Sesion. "));
				
				UE_LOG(LogTemp, Error, TEXT("JOIN SESSION CHECK USER NAME: %s"), *curPlayerName);


				// attempt to join the session
				const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
				joinSessionCompletedHandle = onlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(joinSessionCompletedDelegate);
				
				const FSessionInfo& SelectedSession = SessionList[SessionIndex];
				FName SessionName(*SelectedSession.SessionName);
				FOnlineSessionSearchResult SearchResult = searchSettings->SearchResults[SessionIndex];

				onlineSessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), SessionName, SearchResult);
				// onlineSessionInterface->JoinSession(0, SessionName, SearchResult);
			}
		}
	}
}

void URLInstance::joinSessionCompleted(FName sessionName, EOnJoinSessionCompleteResult::Type joinResult)
{
	UE_LOG(LogTemp, Error, TEXT("Failed to join session: %d"), static_cast<int32>(joinResult));
	sessionAlreadyEnded = false;
	if (IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr onlineSessionInterface = onlineSubsystem->GetSessionInterface())
		{
			onlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(joinSessionCompletedHandle);

			bool wasTravelSuccessful = travelToSession(sessionName);
			
			UE_LOG(LogTemp, Error, TEXT("Travel succesful: %d"), wasTravelSuccessful);

			curSessionName = sessionName;
			UE_LOG(LogTemp, Warning, TEXT("Setting the curSessionName: %s"), *curSessionName.ToString());
		}
	}
}

bool URLInstance::travelToSession(FName sessionName)
{
	if (IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr onlineSessionInterface = onlineSubsystem->GetSessionInterface())
		{
			FString connectionInfo;
			if (onlineSessionInterface->GetResolvedConnectString(sessionName, connectionInfo))
			{
				// Travel the client to the server
				APlayerController* playerController = GetWorld()->GetFirstPlayerController();
				playerController->ClientTravel(connectionInfo, TRAVEL_Absolute);
				return true;
			}
			else
			{
				return false; // the connection infromation could not be obtained
			}
		}
	}

	// the client was unable to travel
	return false;
}

void URLInstance::travelBackToLobby()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("GI: travel back"));
		PlayerController->ClientTravel("Game/HighLevel/lobbylevel/LobbyLevel", TRAVEL_Absolute);
	}
}
void URLInstance::endSession()
{
	UE_LOG(LogTemp, Error, TEXT("Ending session"));

	if (!sessionAlreadyEnded)
	{
		UE_LOG(LogTemp, Error, TEXT("END SESSION CHECK USER NAME: %s"), *curPlayerName);

		sessionAlreadyEnded = true;
		if (IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
		{
			if (IOnlineSessionPtr onlineSessionInterface = onlineSubsystem->GetSessionInterface())
			{
				UE_LOG(LogTemp, Warning, TEXT("End session with name: %s"), *curSessionName.ToString());

				endSessionCompletedHandle = onlineSessionInterface->AddOnEndSessionCompleteDelegate_Handle(endSessionCompletedDelegate);
				bool ableToEnd = onlineSessionInterface->EndSession(curSessionName);

				UE_LOG(LogTemp, Error, TEXT("End Session succesful: %d"), ableToEnd);
			}
		}
	}
	
}

void URLInstance::endSessionCompleted(FName sessionName, bool endedSession)
{
	if (IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr onlineSessionInterface = onlineSubsystem->GetSessionInterface())
		{

			UE_LOG(LogTemp, Error, TEXT("GI: end session complete: %d"), endedSession);
			onlineSessionInterface->ClearOnEndSessionCompleteDelegate_Handle(endSessionCompletedHandle);

			travelBackToLobby();

			destroySession(sessionName);
		}
	}
}

void URLInstance::destroySession(FName sessionName)
{
	if (IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr onlineSessionInterface = onlineSubsystem->GetSessionInterface())
		{
			UE_LOG(LogTemp, Error, TEXT("Destroying Session"));
			destroySessionCompletedHandle = onlineSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(destroySessionCompletedDelegate);
			onlineSessionInterface->DestroySession(sessionName);
		}
	}
}

void URLInstance::destroySessionCompleted(FName sessionName, bool destroyedSession)
{
	if (IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr onlineSessionInterface = onlineSubsystem->GetSessionInterface())
		{
			onlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(destroySessionCompletedHandle);
			UE_LOG(LogTemp, Error, TEXT("Session destroyed Completed: %d"), destroyedSession);
		}
	}
}

// oficial copy:
void URLInstance::createSaveFile()
{
	URLSaveGame* dataToSave = Cast<URLSaveGame>(UGameplayStatics::CreateSaveGameObject(URLSaveGame::StaticClass()));
	if (dataToSave != nullptr)
	{
		curPlayerName = generateRandomUserString();
		dataToSave->playerNameSaved = curPlayerName;

		UE_LOG(LogTemp, Error, TEXT("create save file, user name: %s"), *curPlayerName);
		UGameplayStatics::SaveGameToSlot(dataToSave, "Slot1", 0);
	}
}

FString URLInstance::generateRandomUserString()
{
	FString Base = TEXT("user");
	for (int i = 0; i < 6; ++i)
	{
		int32 RandomDigit = FMath::RandRange(0, 9);
		Base += FString::FromInt(RandomDigit);
	}
	return Base;
}

void URLInstance::saveGame()
{
    URLSaveGame* dataToSave = Cast<URLSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));
    if (dataToSave == nullptr)
    {
        dataToSave = Cast<URLSaveGame>(UGameplayStatics::CreateSaveGameObject(URLSaveGame::StaticClass()));
    }

    if (dataToSave != nullptr)
    { 
        dataToSave->playerNameSaved = curPlayerName;
        UGameplayStatics::SaveGameToSlot(dataToSave, "Slot1", 0);
		UE_LOG(LogTemp, Warning, TEXT("game saved"));
    }
}
void URLInstance::loadGame()
{
	URLSaveGame* dataToLoad = Cast<URLSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	if (dataToLoad != nullptr)
	{
		if (dataToLoad->playerNameSaved.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("The name is empty."));
			createSaveFile();
		}
		else
		{
			curPlayerName = dataToLoad->playerNameSaved;
			UE_LOG(LogTemp, Error, TEXT("save data load, player name: %s"), *curPlayerName);
		}
	}
	else if (!UGameplayStatics::DoesSaveGameExist("Slot1", 0))
	{
		createSaveFile();
	}

	UE_LOG(LogTemp, Error, TEXT("DOUBLE CHECK USER NAME: %s"), *curPlayerName);

}

EGameModeEnum URLInstance::getCurGameMode() {
	return curGameMode;
}

void URLInstance::setLanguage(ELanguage aLanguage)
{
	gameLanguage = aLanguage;
}

ELanguage URLInstance::getLanguage()
{
	return gameLanguage;
}

FString URLInstance::getName()
{
	UE_LOG(LogTemp, Error, TEXT("GameInstance name: %s"), *curPlayerName);
	return curPlayerName;
}