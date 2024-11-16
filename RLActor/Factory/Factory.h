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
    static UFactory* Get();

    // Method to create an RLActor by name
    UFUNCTION(BlueprintCallable, Category = "RLFactory")
    virtual IRLActor* CreateRLActor(const FString& name);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lobby")
    TSubclass<UFactory> factoryClass = nullptr;

protected:
    // Ensure singleton is initialized
    static void Initialize();

    // Protected constructor for singleton pattern
    UFactory();

private:
    // The singleton instance
    static UFactory* singletonInstance;
};

