#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PiecePreviewMesh.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class APiecePreviewMesh : public AActor
{
    GENERATED_BODY()

public:
    APiecePreviewMesh();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Piece Collision")
    UStaticMeshComponent* previewStaticBody;

};