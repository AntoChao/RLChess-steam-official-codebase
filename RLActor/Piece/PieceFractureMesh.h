#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "PieceFractureMesh.generated.h"

UCLASS(Blueprintable, BlueprintType)
class APieceFractureMesh : public AActor
{
    GENERATED_BODY()

public:
    APieceFractureMesh();

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Piece Fracture")
    UGeometryCollectionComponent* pieceFractureBodyMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Fracture")
    int lifeSpanSegs = 10;

public:
    // Function to apply force to the geometry collection
    UFUNCTION(Server, Reliable, BlueprintCallable, BlueprintCallable, Category = "Piece Fracture")
    void setMaterial(UMaterialInterface* selectedMaterial);

    UFUNCTION(Server, Reliable, BlueprintCallable, BlueprintCallable, Category = "Piece Fracture")
    void applyForce(const FVector& Force);
};