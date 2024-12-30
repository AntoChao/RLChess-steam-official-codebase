// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnvShop.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "../Factory/FactoryPiece.h"
#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Piece/Piece.h"
#include "../Item/Item.h"
#include "EnvSquare.h"
#include "EnvBoard.h"
#include "../Player/PlayerCharacter.h"

AEnvShop::AEnvShop()
{
	bReplicates = true;

	// Create and set up the static mesh components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void AEnvShop::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health.
	DOREPLIFETIME(AEnvShop, productsInShop);
}

void AEnvShop::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Shop: begin play"));
}

FString AEnvShop::GetActorName(ELanguage curLanguage)
{
	return TEXT("shop");
}

FString AEnvShop::GetDescription(ELanguage curLanguage)
{
	return TEXT("a place to recruit army");
}

FString AEnvShop::GetInteractionDescription(ELanguage curLanguage)
{
	return TEXT("buy things");
}


bool AEnvShop::IsAbleToBeInteracted(APlayerCharacter* Sender)
{
	return false;
}

void AEnvShop::BeInteracted(APlayerCharacter* Sender)
{
	return;
}

void AEnvShop::BeUnInteracted(APlayerCharacter* Sender)
{
	return;
}

void AEnvShop::createRandomShop_Implementation()
{

	AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	int numUnitsToSpawn = piecesProductTotalNum; // Number of units you want to spawn

	TArray<AEnvSquare*> allShopSquares;

	AEnvBoard* gameBoard = nullptr;
	if (UWorld* World = GetWorld())
	{
		ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
		if (GameState)
		{
			gameBoard = GameState->getGameBoard();

			if (gameBoard)
			{
				int shopRowSize = gameBoard->getRowSize();
				int shopColumnSize = gameBoard->getColumnSize();

				for (int i = 2; i < shopRowSize - 2; i++)
				{
					for (int j = 2; j < shopColumnSize - 2; j++)
					{
						if (i % 3 != 0 && j % 3 != 0)
						{
							AEnvSquare* curSquare = gameBoard->getSquareAtLocation(FVector2D(i, j));

							if (curSquare->getSquareColorField() == FColor::White)
							{
								allShopSquares.Add(curSquare);
							}
						}
					}
				}
			}
		}
	}

	if (curGameMode)
	{
		UFactoryPiece* pieceFactory = curGameMode->pieceFactoryInstance;

		if (pieceFactory)
		{
			int counter = 1;
			for (AEnvSquare* shopSquare : allShopSquares)
			{
				if (counter > numUnitsToSpawn)
				{
					break;
				}

				FVector productPieceLocation = shopSquare->getPlacementLocation();
				FVector boardCenter = FVector(0.0f, 0.0f, productPieceLocation.Z);
				FRotator productPieceRotation = (boardCenter - productPieceLocation).Rotation();

				APiece* generatedPiece = Cast<APiece>(pieceFactory->createRLActorByCode(counter, productPieceLocation, productPieceRotation));
				IRLProduct* productInterface = Cast<IRLProduct>(generatedPiece);

				if (IsValid(generatedPiece))
				{
					TScriptInterface<IRLProduct> productInstance;
					productInstance.SetObject(generatedPiece);
					productInstance.SetInterface(Cast<IRLProduct>(productInterface));

					productsInShop.Add(productInstance);
				}
				counter++;
			}
		}
	}
}


void AEnvShop::sellProduct_Implementation(APlayerCharacter* player, APiece* specificProduct)
{
	if (player && specificProduct)
	{
		if (player->isEnableToBuyProduct(specificProduct))
		{
			refillProduct(specificProduct);

			player->payProduct(specificProduct);
			player->receiveProduct(specificProduct);
			productsInShop.Remove(specificProduct);
		}
	}
}

void AEnvShop::refillProduct_Implementation(APiece* specificProduct)
{
	APiece* pieceToDuplicate = specificProduct;
	if (pieceToDuplicate)
	{
		// Get the class of the actor you want to duplicate
		UClass* ActorClass = pieceToDuplicate->GetClass();

		// Get the world in which to spawn the new actor
		UWorld* World = GetWorld();
		if (World)
		{
			// Setup the spawn parameters
			FActorSpawnParameters SpawnParams;

			// Specify the location and rotation for the new actor
			FVector Location = pieceToDuplicate->GetActorLocation();
			FRotator Rotation = pieceToDuplicate->GetActorRotation();

			// Spawn the new actor
			AActor* NewActor = World->SpawnActor<AActor>(ActorClass, Location, Rotation, SpawnParams);
			
			if (NewActor)
			{
				IRLProduct* productInterface = Cast<IRLProduct>(NewActor);

				TScriptInterface<IRLProduct> productInstance;
				productInstance.SetObject(NewActor);
				productInstance.SetInterface(Cast<IRLProduct>(productInterface));

				productsInShop.Add(productInstance);

			}

		}
	}
}

void AEnvShop::refreshShop_Implementation(APlayerCharacter* player)
{
	productsInShop.Empty();
	createRandomShop();
}

void AEnvShop::closeShop_Implementation()
{
	for (TScriptInterface<IRLProduct>& Product : productsInShop)
	{
		if (Product)
		{
			AActor* Actor = Cast<AActor>(Product.GetObject());
			if (Actor)
			{
				Actor->Destroy();
			}
		}
	}

	productsInShop.Empty();
}

// sell all pieces for ai
void AEnvShop::fullFill_Implementation(APlayerCharacter* controlledPlayer)
{
	APiece* randomProduct = selectRandomProduct();
	while (controlledPlayer->isEnableToBuyProduct(randomProduct))
	{
		if (randomProduct)
		{
			randomProduct->inShopInteractedEffect(controlledPlayer);
		}
		randomProduct = selectRandomProduct();
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("GAMEMODE BEGIN PLAY"));

}

APiece* AEnvShop::selectRandomProduct()
{
	int randomInt = FMath::RandRange(0, productsInShop.Num() - 1);
	APiece* randomPiece = Cast<APiece>(productsInShop[randomInt].GetObject());
	return randomPiece;
}