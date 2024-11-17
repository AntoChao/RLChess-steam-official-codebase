#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Factory.generated.h"

UCLASS(Blueprintable)
class UFactory: public UObject
{
	GENERATED_BODY()
	
public:
    // Access the singleton instance
    static UFactory* get();

    // Method to create an RLActor by name
    UFUNCTION(BlueprintCallable, Category = "RLFactory")
    virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lobby")
    TSubclass<UFactory> factoryClass = nullptr;

    // Protected constructor for singleton pattern
    UFactory();

protected:
    // Ensure singleton is initialized
    static void initialize();

private:
    // The singleton instance
    static UFactory* singletonInstance;
};

