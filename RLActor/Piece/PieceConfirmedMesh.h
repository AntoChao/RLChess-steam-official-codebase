#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceConfirmedMesh.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class APieceConfirmedMesh : public AActor
{
    GENERATED_BODY()

public:
    APieceConfirmedMesh();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Piece Collision")
    UStaticMeshComponent* confirmedStaticBody;

};