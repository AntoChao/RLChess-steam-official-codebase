// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnvShop.h"
#include "../Factory/FactoryPiece.h"
#include "../../RLHighLevel/GameplayGameMode.h"
#include "../Piece/Piece.h"
#include "../Player/PlayerCharacter.h"

AEnvShop::AEnvShop()
{
	piecesInShop.SetNum(piecesProductTotalNum);
	createRandomShop();

	if (shopWidgetClass)
	{
		shopHUD = CreateWidget<UShopWidget>(GetWorld(), shopWidgetClass);
		shopHUD->setAllProducts(piecesInShop);
	}
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
	return true;
}

void AEnvShop::BeInteracted(APlayerCharacter* Sender)
{
	showShopHUDToPlayer(Sender);
}

void AEnvShop::BeUnInteracted(APlayerCharacter* Sender)
{
	return;
}

void AEnvShop::createRandomShop()
{
	AGameplayGameMode* curGameMode = Cast<AGameplayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (curGameMode)
	{
		UFactoryPiece* pieceFactory = curGameMode->pieceFactoryInstance;

		if (pieceFactory)
		{
			for (APiece* eachPiece : piecesInShop)
			{
				if (!IsValid(eachPiece))
				{
					eachPiece = Cast<APiece>(pieceFactory->createRandom(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f)));
				}
			}
		}
	}
}

void AEnvShop::showShopHUDToPlayer(APlayerCharacter* player)
{
	if (shopHUD)
	{
		player->attachHUDToViewport(shopHUD);
	}
}

void AEnvShop::sellProduct(APlayerCharacter* player, APiece* specificPiece)
{
	if (player->isAbleToRecruit(specificPiece))
	{
		player->recruitArmy(specificPiece);
		piecesInShop.Remove(specificPiece);
	}
}

void AEnvShop::refreshShop(APlayerCharacter* player)
{
	createRandomShop();
}