#include "PieceConfirmedMesh.h"

#include "Components/StaticMeshComponent.h"

APieceConfirmedMesh::APieceConfirmedMesh()
{
    // Enable network replication
    bReplicates = true;

    // Create and set the root component as a SceneComponent
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(Root);

    // Create the StaticMeshComponent and attach it to the root
    confirmedStaticBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Confirmed Static Body"));

    confirmedStaticBody->SetupAttachment(RootComponent);
    confirmedStaticBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    confirmedStaticBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}