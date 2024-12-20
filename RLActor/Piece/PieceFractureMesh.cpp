#include "PieceFractureMesh.h"


APieceFractureMesh::APieceFractureMesh()
{
    // Enable network replication
    bReplicates = true;

    // Create and set the root component as a SceneComponent
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(Root);

    // Create the Geometry Collection Component
    pieceFractureBodyMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("PieceFractureMeshComponent"));
    pieceFractureBodyMesh->SetupAttachment(RootComponent);

    // Set the actor to automatically destroy after 10 seconds
    SetLifeSpan(lifeSpanSegs);
}

void APieceFractureMesh::setMaterial(UMaterialInterface* selectedMaterial)
{
    if (selectedMaterial && pieceFractureBodyMesh)
    {
        int32 MaterialCount = pieceFractureBodyMesh->GetNumMaterials();
        for (int32 Index = 0; Index < MaterialCount; ++Index)
        {
            pieceFractureBodyMesh->SetMaterial(Index, selectedMaterial);
        }
    }

    pieceFractureBodyMesh->SetVisibility(true);
}

void APieceFractureMesh::applyForce(const FVector& Force)
{
    if (pieceFractureBodyMesh)
    {
        // pieceFractureBodyMesh->AddForce(Force, NAME_None, true);
        pieceFractureBodyMesh->AddImpulse(Force, NAME_None, true);
    }
}