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

    virtual FString GetActorName() override;

    virtual FString GetDescription() override;

    virtual bool IsAbleToBeInteracted(APlayerCharacter* Sender) override;

    UFUNCTION(BlueprintCallable, Category = "Piece")
    virtual void BeInteracted(APlayerCharacter* Sender) override;

    virtual void BeUnInteracted(APlayerCharacter* Sender) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Piece Collision")
    UStaticMeshComponent* previewStaticBody;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString previewPieceName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Stats")
    FString previewPieceDescription;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Piece Collision")
    AEnvSquare* previewSquare;

public:
    UFUNCTION(BlueprintCallable, Category = "Piece Preview")
    void setPreviewSquare(AEnvSquare* theSquare);

    UFUNCTION(BlueprintCallable, Category = "Piece Preview")
    AEnvSquare* getPreviewSquare();

};