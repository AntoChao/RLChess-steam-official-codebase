#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "../../../CommonEnum.h"

#include "HUDLobby.generated.h"

UCLASS()
class UHUDLobby : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void refreshSearchResult(const TArray<FSessionInfo>& allSessionList);
};

