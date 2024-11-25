#pragma once

#include "CoreMinimal.h"
#include "RLFactory.h"
#include "../Piece/Piece.h"
#include "FactoryPiece.generated.h"

UCLASS(Blueprintable)
class UFactoryPiece : public URLFactory
{
	GENERATED_BODY()
	
public:
	virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation) override;

	virtual AActor* createRandom(FVector aLocation, FRotator aRotation) override;
};

