// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RLChessGameMode.h"

#include "GameplayGameMode.generated.h"

class UFactoryPlayer;
class UFactoryEnvironment;
class UFactoryItem;
class UFactoryPiece;

class AEnvBoard;
class AEnvShop;

class APlayerRLController;

UCLASS(minimalapi)
class AGameplayGameMode : public ARLChessGameMode
{
	GENERATED_BODY()

public:
	AGameplayGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

    UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
    void debugFunctionOne(); // gamemode check all players login
    UFUNCTION(BlueprintImplementableEvent, Category = "debugFunction")
    void debugFunctionTwo(); // gamemode all player start set up round

protected:

	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    int numberOfAIPlayers = 2;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    int numberOfHumanPlayers = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    TArray<FColor> allRestColor = {
        FColor::Red,
        FColor::Green,
        FColor::Blue,
        FColor::Yellow,
        FColor::Purple
    };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
	TArray<AController*> allPlayerControllers;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    TArray<FString> allNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
	TArray<APlayerCharacter*> allPlayers;

	// all factory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
		TSubclassOf<UFactoryPlayer> playerFactoryClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
		TSubclassOf<UFactoryEnvironment> envFactoryClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
		TSubclassOf<UFactoryItem> itemFactoryClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
		TSubclassOf<UFactoryPiece> pieceFactoryClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
		UFactoryPlayer* playerFactoryInstance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
		UFactoryEnvironment* environmentFactoryInstance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
		UFactoryItem* itemFactoryInstance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Class")
		UFactoryPiece* pieceFactoryInstance = nullptr;

	// all locations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
	float playerSpawnHeight = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
    AEnvBoard* serverBoard;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
    FVector boardLocation = FVector::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
    FRotator boardRotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
    AEnvShop* serverShop;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Stats")
    FVector shopLocation = FVector::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Stats")
    FRotator shopRotation = FRotator::ZeroRotator;

    // round start
    UFUNCTION(BlueprintCallable, Category = "GameMode")
    void updateAllPlayersBody(TArray<APlayerCharacter*> allPlayerBody);

    UFUNCTION(BlueprintCallable, Category = "GameMode")
        void startIfAllPlayerLoggedIn();

protected:
	// set up
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void setupGame();
	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void setupSingletonClasses();

    // called by gamemode to initialize
    UFUNCTION(Category = "GameMode")
    void startSetUpRound();
    UFUNCTION(Category = "GameMode")
    void startGameplayRound();

    
    UFUNCTION(BlueprintCallable, Category = "GameMode")
    void initAIPlayers();
    UFUNCTION(BlueprintCallable, Category = "GameMode")
    void spawnMap();
    UFUNCTION(BlueprintCallable, Category = "GameMode")
    void initMap();

    UFUNCTION(BlueprintCallable, Category = "GameMode")
    void setPlayerBench();
    UFUNCTION(Category = "GameMode")
    void setPlayerInitLocation();

    UFUNCTION(Category = "GameMode")
    void startPlayerSetUpTime();
    UFUNCTION(Category = "GameMode")
    void endPlayerSetUpTime();
    UFUNCTION(Category = "GameMode")
    void startPlayerPreparePhase();
    UFUNCTION(Category = "GameMode")
    void startPieceMovingPhase();

    UFUNCTION(Category = "GameMode")
    void countDown();
    
public :
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    int curRestTime;
    
protected :
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    FTimerHandle countDownHandle;
    
    // manage player setup time
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    bool isPlayerSetupTime = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    FTimerHandle playerSetUpTimerHandle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    int playerSetUpTimerSegs = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    FTimerHandle playerTurnTimerHandle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    int turnTimeInSegs = 10; // in segs

    // manage player round
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    int roundCounter = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Round Manager")
    FTimerHandle playerPrepareTimerHandle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    int playerPrepareTimerSegs = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    FTimerHandle piecesMovedTimerHandle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    int piecesMovedTimerSegs = 5;

    // player died 
    

    // game end
public:
    UFUNCTION(BlueprintCallable, Category = "GameMode")
    void checkGameEnd();
    
    UFUNCTION(BlueprintCallable, Category = "GameMode")
    bool checkIfGameEnd();
    UFUNCTION(BlueprintCallable, Category = "GameMode")
    void endGameplayGameMode();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    bool ended = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    AController* winner = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Manager")
    FString winnerName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Main")
    TSubclassOf<UUserWidget> gameResultHUDClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Main")
    UUserWidget* gameResultHUD;


    UFUNCTION(BlueprintCallable, Category = "GameMode")
    TArray<AController*> GetAllPlayerControllers();
};



