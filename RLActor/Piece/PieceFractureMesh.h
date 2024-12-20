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

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Piece Fracture")
    UGeometryCollectionComponent* pieceFractureBodyMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Fracture")
    int lifeSpanSegs = 10;
public:
    // Function to apply force to the geometry collection
    UFUNCTION(BlueprintCallable, BlueprintCallable, Category = "Piece Fracture")
    void setMaterial(UMaterialInterface* selectedMaterial);

    UFUNCTION(BlueprintCallable, BlueprintCallable, Category = "Piece Fracture")
    void applyForce(const FVector& Force);
};