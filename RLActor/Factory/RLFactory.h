#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "Math/UnrealMathUtility.h"
#include "RLFactory.generated.h"

UCLASS(Blueprintable)
class URLFactory: public UObject
{
	GENERATED_BODY()
	

protected:
    // Protected constructor for singleton pattern
    URLFactory();

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    FActorSpawnParameters SpawnParams;
   
public:
    // Method to create an RLActor by name
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Map Manager")
    UWorld* gameWorld = nullptr;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Map Manager")
    AActor* createdActor = nullptr;
    
    UFUNCTION(BlueprintCallable, Category = "RLFactory")
    virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation);

    UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "RLFactory")
    void spawnRLActor(const FString& name, FVector aLocation, FRotator aRotation);

    virtual void spawnRLActor_Implementation(const FString& name, FVector aLocation, FRotator aRotation);

    UFUNCTION(BlueprintCallable, Category = "RLFactory")
    virtual AActor* createRandom(FVector aLocation, FRotator aRotation);

    UFUNCTION(BlueprintCallable, Category = "RLFactory")
    virtual AActor* createRLActorByCode(int code, FVector aLocation, FRotator aRotation);

};

