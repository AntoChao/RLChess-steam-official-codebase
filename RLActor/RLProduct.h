// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RLProduct.generated.h"

UINTERFACE(MinimalAPI)
class URLProduct : public UInterface
{
    GENERATED_BODY()
};

class IRLProduct
{
    GENERATED_BODY()

public:
    // Get the cost of the product
    virtual int GetProductCost() = 0;

    // Get the name of the product
    virtual FString GetProductName() = 0;

    // Get the 2D image of the product
    virtual UTexture2D* GetProductImage() = 0;
};