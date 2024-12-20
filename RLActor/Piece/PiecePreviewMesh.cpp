#include "PiecePreviewMesh.h"

#include "Components/StaticMeshComponent.h"

APiecePreviewMesh::APiecePreviewMesh()
{
    // Enable network replication
    bReplicates = true;

    // Create and set the root component as a SceneComponent
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(Root);

    // Create the StaticMeshComponent and attach it to the root
    previewStaticBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destructable Body"));

    previewStaticBody->SetupAttachment(RootComponent);
    previewStaticBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    previewStaticBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}