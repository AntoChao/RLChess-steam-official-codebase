#include "PlayerRLState.h"
#include "Net/UnrealNetwork.h"

APlayerRLState::APlayerRLState()
{
    bReplicates = true;
}

void APlayerRLState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate properties
    DOREPLIFETIME(APlayerRLState, isReady);
    DOREPLIFETIME(APlayerRLState, playerName);
    DOREPLIFETIME(APlayerRLState, playerColor);
    DOREPLIFETIME(APlayerRLState, characterName);
}