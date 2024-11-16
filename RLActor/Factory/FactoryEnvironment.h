#pragma once

#include "CoreMinimal.h"
#include "Factory.h"
#include "FactoryEnvironment .generated.h"

UCLASS(Blueprintable)
class UFactoryEnvironment : public UFactory
{
	GENERATED_BODY()
	
public:
	virtual IRLActor* CreateRLActor(const FString& name) override;
};

