#pragma once

#include "CoreMinimal.h"
#include "Factory.h"
#include "FactoryItem.generated.h"

UCLASS(Blueprintable)
class UFactoryItem : public UFactory
{
	GENERATED_BODY()
	
public:
	virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation) override;
};

