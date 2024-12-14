#pragma once

#include "CoreMinimal.h"
#include "RLFactory.h"

#include "FactoryEnvironment.generated.h"

class AEnvBoard;
class AEnvShop;
class AEnvSquare;

UCLASS(Blueprintable)
class UFactoryEnvironment : public URLFactory
{
	GENERATED_BODY()
	
public:
    virtual void spawnRLActor_Implementation(const FString& name, FVector aLocation, FRotator aRotation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Stats")
		TSubclassOf<AEnvBoard> boardClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
		TSubclassOf<AEnvSquare> blackSquareClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
		TSubclassOf<AEnvSquare> whiteSquareClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
		TSubclassOf<AEnvSquare> benchSquareClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
		TSubclassOf<AEnvSquare> invisibleSquare;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Square Stats")
		TSubclassOf<AEnvShop> shopClass;

};

