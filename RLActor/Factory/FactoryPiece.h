#pragma once

#include "CoreMinimal.h"
#include "Factory.h"
#include "FactoryPiece.generated.h"

UCLASS(Blueprintable)
class UFactoryPiece : public UFactory
{
	GENERATED_BODY()
	
public:
	virtual IRLActor* CreateRLActor(const FString& name) override;
};

