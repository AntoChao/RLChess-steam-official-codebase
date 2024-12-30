#include "SimulatedBoardState.h"
#include "../Piece/Piece.h"

USimulatedBoardState::USimulatedBoardState() {
    // Initialize game state if needed
}

void USimulatedBoardState::ApplyMove(FSimulatedPiece& SimPiece, const FVector2D& Target) {
    SimPiece.PieceLocation = Target;
}

void USimulatedBoardState::ResolveConflicts() {
    for (int32 PlayerIndex = PlayerPieces.Num() - 1; PlayerIndex >= 0; --PlayerIndex) {
        for (int32 OpponentIndex = OpponentPieces.Num() - 1; OpponentIndex >= 0; --OpponentIndex) {
            FSimulatedPiece myPiece = PlayerPieces[PlayerIndex];
            FSimulatedPiece oppoPiece = OpponentPieces[OpponentIndex];
            if (myPiece.PieceLocation == oppoPiece.PieceLocation) {
                if (myPiece.Piece->getLevel() > oppoPiece.Piece->getLevel()) {
                    // Remove the opponent piece
                    OpponentPieces.RemoveAt(OpponentIndex);
                }
                else {
                    // Remove the player piece and break out of the inner loop
                    PlayerPieces.RemoveAt(PlayerIndex);
                }
            }
        }
    }
}

bool USimulatedBoardState::IsGameOver() const {
    return PlayerPieces.Num() == 0 || OpponentPieces.Num() == 0;
}

int32 USimulatedBoardState::EvaluateState() const {
    int32 Score = 0;
    for (const FSimulatedPiece& SimPiece : PlayerPieces) {
        Score += SimPiece.Piece->getLevel();
        // Score += SimPiece.Piece->calculatePossibleMove().Num(); // Reward mobility
    }
    for (const FSimulatedPiece& SimPiece : OpponentPieces) {
        Score -= SimPiece.Piece->getLevel();
        // Score -= SimPiece.Piece->calculatePossibleMove().Num(); // Penalize opponent mobility
    }
    return Score;
}