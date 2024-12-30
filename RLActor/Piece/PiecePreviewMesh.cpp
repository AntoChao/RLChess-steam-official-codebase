#include "PiecePreviewMesh.h"

#include "Components/StaticMeshComponent.h"

APiecePreviewMesh::APiecePreviewMesh()
{
    // Create and set the root component as a SceneComponent
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(Root);

    // Create the StaticMeshComponent and attach it to the root
    previewStaticBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destructable Body"));

    previewStaticBody->SetupAttachment(RootComponent);
    previewStaticBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    
    previewStaticBody->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Enables collision queries (traces), not physics collisions
    previewStaticBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Ignore all channels by default
    previewStaticBody->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block); // Block camera traces
    previewStaticBody->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block); // Block visibility traces
}

FString APiecePreviewMesh::GetActorName(ELanguage curLanguage)
{
	switch (curLanguage)
	{
	case ELanguage::EEnglish:
	{
		return piecePrevoewNameEng;
	}
	case ELanguage::EChinese:
	{
		return piecePreviewNameChi;
	}
	case ELanguage::ESpanish:
	{
		return piecePreviewNameSpa;
	}
	}
	return piecePrevoewNameEng;
}

FString APiecePreviewMesh::GetDescription(ELanguage curLanguage)
{
	switch (curLanguage)
	{
	case ELanguage::EEnglish:
	{
		return piecePreviewDescriptionEng;
	}
	case ELanguage::EChinese:
	{
		return piecePreviewDescriptionChi;
	}
	case ELanguage::ESpanish:
	{
		return piecePreviewDescriptionSpa;
	}
	}
	return piecePreviewDescriptionEng;
}

FString APiecePreviewMesh::GetInteractionDescription(ELanguage curLanguage)
{
	switch (curLanguage)
	{
	case ELanguage::EEnglish:
	{
		return piecePreviewInteractDescEng;
	}
	case ELanguage::EChinese:
	{
		return piecePreviewInteractDescChi;
	}
	case ELanguage::ESpanish:
	{
		return piecePreviewInteractDescSpa;
	}
	}
	return piecePreviewInteractDescEng;
}

bool APiecePreviewMesh::IsAbleToBeInteracted(APlayerCharacter* Sender)
{
    return true;
}

void APiecePreviewMesh::BeInteracted(APlayerCharacter* Sender)
{
    return;
}
void APiecePreviewMesh::BeUnInteracted(APlayerCharacter* Sender)
{
    return;
}

void APiecePreviewMesh::BeginPlay()
{
    Super::BeginPlay();

}

void APiecePreviewMesh::setMaterial(UMaterialInterface* selectedMaterial)
{
    if (selectedMaterial && previewStaticBody)
    {
        int32 MaterialCount = previewStaticBody->GetNumMaterials();
        for (int32 Index = 0; Index < MaterialCount; ++Index)
        {
            previewStaticBody->SetMaterial(Index, selectedMaterial);
        }
    }

    previewStaticBody->SetVisibility(true);
}

void APiecePreviewMesh::setPreviewSquare(AEnvSquare* theSquare)
{
    previewSquare = theSquare;
}

AEnvSquare* APiecePreviewMesh::getPreviewSquare()
{
    return previewSquare;
}