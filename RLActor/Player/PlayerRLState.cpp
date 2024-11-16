#include "PlayerRLState.h"
#include "Net/UnrealNetwork.h

void APlayerRLState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate properties
    DOREPLIFETIME(APlayerRLState, isReady);
    DOREPLIFETIME(APlayerRLState, playerName);
    DOREPLIFETIME(APlayerRLState, characterName);
    DOREPLIFETIME(APlayerRLState, playerPiecesName);
    DOREPLIFETIME(APlayerRLState, playerColor);
}