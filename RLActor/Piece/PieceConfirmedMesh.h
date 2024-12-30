#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "../RLActor.h"
#include "PieceConfirmedMesh.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class APieceConfirmedMesh : public AActor, public IRLActor
{
    GENERATED_BODY()

public:
    APieceConfirmedMesh();

    virtual void BeginPlay() override;

    virtual FString GetActorName(ELanguage curLanguage) override;

    virtual FString GetDescription(ELanguage curLanguage) override;

    virtual FString GetInteractionDescription(ELanguage curLanguage) override;

    virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

    virtual void BeInteracted(APlayerCharacter* Sender) override;

    virtual void BeUnInteracted(APlayerCharacter* Sender) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Piece Collision")
    UStaticMeshComponent* confirmedStaticBody;

public:
    UFUNCTION(BlueprintCallable, Category = "Piece Preview")
    void setMaterial(UMaterialInterface* selectedMaterial);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString pieceConfirmNameEng;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString pieceConfirmNameChi;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString pieceConfirmNameSpa;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString pieceConfirmDescriptionEng;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString pieceConfirmDescriptionChi;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString pieceConfirmDescriptionSpa;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString pieceConfirmInteractDescEng;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString pieceConfirmInteractDescChi;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString pieceConfirmInteractDescSpa;
};