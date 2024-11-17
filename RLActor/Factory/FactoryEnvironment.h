#pragma once

#include "CoreMinimal.h"
#include "Factory.h"
#include "FactoryEnvironment .generated.h"

UCLASS(Blueprintable)
class UFactoryEnvironment : public UFactory
{
	GENERATED_BODY()
	
public:
	virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation) override;
};

