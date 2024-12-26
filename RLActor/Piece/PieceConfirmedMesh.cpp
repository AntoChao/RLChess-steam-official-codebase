#include "PieceConfirmedMesh.h"

#include "Components/StaticMeshComponent.h"

APieceConfirmedMesh::APieceConfirmedMesh()
{
    // Create and set the root component as a SceneComponent
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(Root);

    // Create the StaticMeshComponent and attach it to the root
    confirmedStaticBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Confirmed Static Body"));

    confirmedStaticBody->SetupAttachment(RootComponent);
    confirmedStaticBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    confirmedStaticBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}


void APieceConfirmedMesh::BeginPlay()
{
    Super::BeginPlay();

    if (selectedMaterial && confirmedStaticBody)
    {
        int32 MaterialCount = confirmedStaticBody->GetNumMaterials();
        for (int32 Index = 0; Index < MaterialCount; ++Index)
        {
            confirmedStaticBody->SetMaterial(Index, selectedMaterial);
        }
    }

    confirmedStaticBody->SetVisibility(true);
}