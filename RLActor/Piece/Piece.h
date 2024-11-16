// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../RLActor.h"
#include "Piece.generated.h"

UCLASS(BlueprintType, Blueprintable)
class APiece : public AActor, public IRLActor
{
	GENERATED_BODY()

public:
	APiece();

public:
	virtual FName GetActorName() const override;

	virtual FText GetDescription() const override;

	virtual bool IsAbleToInteract(RLPlayer* Sender) const override;

	virtual void BeInteracted(RLPlayer* Sender) override;

	virtual void BeUnInteracted(RLPlayer* Sender) override;

};

