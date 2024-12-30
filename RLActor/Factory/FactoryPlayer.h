#pragma once

#include "CoreMinimal.h"
#include "RLFactory.h"
#include "FactoryPlayer.generated.h"

class APlayerCharacter;
class AAIRLController;

UCLASS(Blueprintable)
class UFactoryPlayer : public URLFactory
{
	GENERATED_BODY()
	
public:
	virtual void spawnRLActor_Implementation(const FString& name, FVector aLocation, FRotator aRotation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player TSubclass")
	TSubclassOf<APlayerCharacter> merchantClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player TSubclass")
	TSubclassOf<APlayerCharacter> dieClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player TSubclass")
		TSubclassOf<AAIRLController> aiControllerClass = nullptr;
};

