#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "RLFactory.generated.h"

UCLASS(Blueprintable)
class URLFactory: public UObject
{
	GENERATED_BODY()
	
public:
    // Method to create an RLActor by name
    UFUNCTION(BlueprintCallable, Category = "RLFactory")
    virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation);

    UFUNCTION(BlueprintCallable, Category = "RLFactory")
    virtual AActor* createRandom(FVector aLocation, FRotator aRotation);

protected:
    // Protected constructor for singleton pattern
    URLFactory();

    FActorSpawnParameters SpawnParams;
};

