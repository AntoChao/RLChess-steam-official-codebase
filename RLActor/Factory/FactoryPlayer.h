#pragma once

#include "CoreMinimal.h"
#include "Factory.h"
#include "FactoryPlayer.generated.h"

UCLASS(Blueprintable)
class UFactoryPlayer : public UFactory
{
	GENERATED_BODY()
	
public:
	virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation) override;
};

