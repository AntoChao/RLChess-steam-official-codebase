#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Piece/Piece.h"
#include "ShopWidget.generated.h"

class AEnvShop;

UCLASS()
class UShopWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Widget")
	AEnvShop* shopReference;

};

