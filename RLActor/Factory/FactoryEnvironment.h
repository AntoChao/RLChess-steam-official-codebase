#pragma once

#include "CoreMinimal.h"
#include "RLFactory.h"
#include "FactoryEnvironment.generated.h"

class AEnvBoard;

UCLASS(Blueprintable)
class UFactoryEnvironment : public URLFactory
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
	TSubclassOf<AEnvBoard> boardClass = nullptr;

public:
	virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation) override;
};

