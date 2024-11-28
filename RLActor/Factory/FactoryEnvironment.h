#pragma once

#include "CoreMinimal.h"
#include "RLFactory.h"
#include "../Environment/EnvBoard.h"
#include "FactoryEnvironment.generated.h"

class AEnvBoard;

UCLASS(Blueprintable)
class UFactoryEnvironment : public URLFactory
{
	GENERATED_BODY()
	
public:
    virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
		TSubclassOf<AEnvBoard> boardClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
		TSubclassOf<AEnvSquare> blackSquareClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
		TSubclassOf<AEnvSquare> whiteSquareClass;
};

