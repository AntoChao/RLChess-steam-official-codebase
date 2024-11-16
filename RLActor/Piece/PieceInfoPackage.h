#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PieceInfoPackage.generated.h"

UCLASS(config=Game)
class UPieceInfoPackage : public UObject
{
	GENERATED_BODY()

public:
	UPieceInfoPackage();
};

