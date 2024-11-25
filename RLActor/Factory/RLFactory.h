#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "RLFactory.generated.h"

UCLASS(Blueprintable)
class URLFactory: public UObject
{
	GENERATED_BODY()
	
public:
    // Access the singleton instance
    static URLFactory* get();

    // Method to create an RLActor by name
    UFUNCTION(BlueprintCallable, Category = "RLFactory")
    virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation);

    UFUNCTION(BlueprintCallable, Category = "RLFactory")
    virtual AActor* createRandom(FVector aLocation, FRotator aRotation);

    static TSubclassOf<URLFactory> factoryClass;

    // Protected constructor for singleton pattern
    URLFactory();

    static URLFactory* singletonInstance;

protected:
    // Ensure singleton is initialized
    static void initialize();
};

