#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../RLActor.h"

#include "PiecePreviewMesh.generated.h"

class UStaticMeshComponent;
class AEnvSquare;

UCLASS(Blueprintable, BlueprintType)
class APiecePreviewMesh : public AActor, public IRLActor
{
    GENERATED_BODY()

public:
    APiecePreviewMesh();

    virtual FString GetActorName(ELanguage curLanguage) override;

    virtual FString GetDescription(ELanguage curLanguage) override;
    
    virtual FString GetInteractionDescription(ELanguage curLanguage) override;

    virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

    UFUNCTION(BlueprintCallable, Category = "Piece")
    virtual void BeInteracted(APlayerCharacter* Sender) override;

    virtual void BeUnInteracted(APlayerCharacter* Sender) override;
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Piece Collision")
    UStaticMeshComponent* previewStaticBody;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Piece Collision")
    AEnvSquare* previewSquare;
public:
    UFUNCTION(BlueprintCallable, Category = "Piece Preview")
    void setMaterial(UMaterialInterface* selectedMaterial);

    UFUNCTION(BlueprintCallable, Category = "Piece Preview")
    void setPreviewSquare(AEnvSquare* theSquare);

    UFUNCTION(BlueprintCallable, Category = "Piece Preview")
    AEnvSquare* getPreviewSquare();

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString piecePrevoewNameEng;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString piecePreviewNameChi;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString piecePreviewNameSpa;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString piecePreviewDescriptionEng;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString piecePreviewDescriptionChi;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString piecePreviewDescriptionSpa;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString piecePreviewInteractDescEng;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString piecePreviewInteractDescChi;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString piecePreviewInteractDescSpa;
};