// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnvShop.h"
#include "../Factory/FactoryPiece.h"
#include "../../RLHighLevel/GameplayGameMode.h"
#include "../Piece/Piece.h"
#include "../Item/Item.h"
#include "../Player/PlayerCharacter.h"

AEnvShop::AEnvShop()
{
	// Create and set up the static mesh components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	shopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shop Mesh"));
	shopMesh->SetupAttachment(RootComponent);
}

void AEnvShop::BeginPlay()
{
	Super::BeginPlay();

	// Shop pieces
	createRandomShop();

}

FString AEnvShop::GetActorName()
{
	return TEXT("shop");
}

FString AEnvShop::GetDescription()
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

void AEnvShop::createRandomShop()
{
	AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	int numUnitsToSpawn = piecesProductTotalNum; // Number of units you want to spawn
	int attempts = 0;

	UMapManager* mapManager = UMapManager::get();
	if (mapManager && curGameMode)
	{
		AEnvBoard* gameBoard = mapManager->getGameBoard();

		UFactoryPiece* pieceFactory = curGameMode->pieceFactoryInstance;

		if (gameBoard && pieceFactory)
		{
			int shopRowSize = gameBoard->getRowSize();
			int shopColumnSize = gameBoard->getColumnSize();

			for (int i = 0; i < shopRowSize; i++)
			{
				for (int j = 0; j < shopColumnSize; j++)
				{
					AEnvSquare* curSquare = gameBoard->getSquareAtLocation(FVector2D(i, j));
					
					if (curSquare->getSquareColorField() == FColor::White)
					{
						FVector productPieceLocation = curSquare->getPlacementLocation();
						FVector boardCenter = FVector(0.0f, 0.0f, productPieceLocation.Z);
						FRotator productPieceRotation = (boardCenter - productPieceLocation).Rotation();
						
						DrawDebugLine(GetWorld(), productPieceLocation, boardCenter, FColor::Green, false, 5, 0, 5);

						APiece* generatedPiece = Cast<APiece>(pieceFactory->createRandom(productPieceLocation, productPieceRotation));
						IRLProduct* productInterface = Cast<IRLProduct>(generatedPiece);

						if (generatedPiece)
						{
							TScriptInterface<IRLProduct> productInstance;
							productInstance.SetObject(generatedPiece);
							productInstance.SetInterface(Cast<IRLProduct>(productInterface));

							productsInShop.Add(productInstance);

						}
						// if (numUnitsToSpawn <= 0) break; // Stop if we've spawned all units

					}
					// if (numUnitsToSpawn <= 0) break; // Stop if we've spawned all units

				}
			
				attempts++;
				// if (attempts > 100) break; // Break out after 100 attempts to prevent infinite loops
			}
		}
	}
}


void AEnvShop::sellProduct(APlayerCharacter* player, TScriptInterface<IRLProduct> specificProduct)
{
	if (player && specificProduct)
	{
		if (player->isEnableToBuyProduct(specificProduct))
		{
			player->payProduct(specificProduct);
			player->receiveProduct(specificProduct);
			productsInShop.Remove(specificProduct);
		}
	}
}

void AEnvShop::refreshShop(APlayerCharacter* player)
{
	productsInShop.Empty();
	createRandomShop();
}

void AEnvShop::closeShop()
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