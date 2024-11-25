#pragma once

#include "CoreMinimal.h"
#include "RLFactory.h"
#include "../Player/PlayerCharacter.h"
#include "FactoryPlayer.generated.h"

UCLASS(Blueprintable)
class UFactoryPlayer : public URLFactory
{
	GENERATED_BODY()
	
public:
	UFactoryPlayer();

	virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player TSubclass")
	TSubclassOf<APlayerCharacter> merchantClass = nullptr;

};

