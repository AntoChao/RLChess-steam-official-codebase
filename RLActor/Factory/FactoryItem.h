#pragma once

#include "CoreMinimal.h"
#include "RLFactory.h"
#include "FactoryItem.generated.h"

UCLASS(Blueprintable)
class UFactoryItem : public URLFactory
{
	GENERATED_BODY()
	
public:
	virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation) override;
};

