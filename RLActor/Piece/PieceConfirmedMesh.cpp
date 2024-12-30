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
}

void APieceConfirmedMesh::setMaterial(UMaterialInterface* selectedMaterial)
{
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

FString APieceConfirmedMesh::GetActorName(ELanguage curLanguage)
{
    switch (curLanguage)
    {
    case ELanguage::EEnglish:
    {
        return pieceConfirmNameEng;
    }
    case ELanguage::EChinese:
    {
        return pieceConfirmNameChi;
    }
    case ELanguage::ESpanish:
    {
        return pieceConfirmNameSpa;
    }
    }
    return pieceConfirmNameEng;
}

FString APieceConfirmedMesh::GetDescription(ELanguage curLanguage)
{
    switch (curLanguage)
    {
    case ELanguage::EEnglish:
    {
        return pieceConfirmDescriptionEng;
    }
    case ELanguage::EChinese:
    {
        return pieceConfirmDescriptionChi;
    }
    case ELanguage::ESpanish:
    {
        return pieceConfirmDescriptionSpa;
    }
    }
    return pieceConfirmDescriptionEng;
}

FString APieceConfirmedMesh::GetInteractionDescription(ELanguage curLanguage)
{
    switch (curLanguage)
    {
    case ELanguage::EEnglish:
    {
        return pieceConfirmInteractDescEng;
    }
    case ELanguage::EChinese:
    {
        return pieceConfirmInteractDescChi;
    }
    case ELanguage::ESpanish:
    {
        return pieceConfirmInteractDescSpa;
    }
    }
    return pieceConfirmInteractDescEng;
}

bool APieceConfirmedMesh::IsAbleToBeInteracted(APlayerCharacter* Sender)
{
    return false;
}

void APieceConfirmedMesh::BeInteracted(APlayerCharacter* Sender)
{
    return;
}

void APieceConfirmedMesh::BeUnInteracted(APlayerCharacter* Sender)
{
    return;
}