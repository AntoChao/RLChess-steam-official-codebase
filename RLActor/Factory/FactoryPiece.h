#pragma once

#include "CoreMinimal.h"
#include "Factory.h"
#include "FactoryPiece.generated.h"

UCLASS(Blueprintable)
class UFactoryPiece : public UFactory
{
	GENERATED_BODY()
	
public:
	virtual AActor* CreateRLActor(const FString& name, FVector aLocation, FRotator aRotation) override;
};

