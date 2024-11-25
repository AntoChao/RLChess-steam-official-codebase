#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Piece/Piece.h"
#include "ShopWidget.generated.h"

UCLASS()
class UShopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Widget")
	TArray<APiece*> allProducts;

public:
	UFUNCTION(BlueprintCallable, Category = "Shop Widget")
	void setAllProducts(TArray<APiece*> products);
};

