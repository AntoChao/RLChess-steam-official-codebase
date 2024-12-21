#include "PieceFractureMesh.h"

#include "Net/UnrealNetwork.h"

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

void APieceFractureMesh::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //Replicate current health.
    DOREPLIFETIME(APieceFractureMesh, pieceFractureBodyMesh);
}

void APieceFractureMesh::setMaterial_Implementation(UMaterialInterface* selectedMaterial)
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

void APieceFractureMesh::applyForce_Implementation(const FVector& Force)
{
    if (pieceFractureBodyMesh)
    {
        // pieceFractureBodyMesh->AddForce(Force, NAME_None, true);
        pieceFractureBodyMesh->AddImpulse(Force, NAME_None, true);
    }
}