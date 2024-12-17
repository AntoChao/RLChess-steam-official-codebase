#include "PieceGuard.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Environment/EnvBoard.h"

APieceGuard::APieceGuard()
{
}

TArray<FVector2D> APieceGuard::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;
    specialPossibleMove.Empty();
    moveMode = EPieceMoveMode::EGround;

    if (!curSquare) // Ensure the guard is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* gameBoard = nullptr;
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            gameBoard = GameState->getGameBoard();
        }
    }

    if (gameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();

        // Step 1: Add the forward move
        FVector2D ForwardDirection = getDirectionVector(pieceDirection);
        FVector2D ForwardMove = CurrentLocation + ForwardDirection;
        if (gameBoard->isValidLocation(ForwardMove) && !gameBoard->isSquareOccupied(ForwardMove))
        {
            PossibleMoves.Add(ForwardMove);
        }

        // Step 2: Teleport to any square in the same row/column as ally pieces

        // Add all horizontal and vertical moves from the ally's location
        TArray<EPieceDirection> TeleportDirections = {
            EPieceDirection::EUp, EPieceDirection::EDown,
            EPieceDirection::ELeft, EPieceDirection::ERight
        };

        for (int x = 0; x < gameBoard->getRowSize(); ++x)
        {
            for (int y = 0; y < gameBoard->getColumnSize(); ++y)
            {
                FVector2D CheckLocation = FVector2D(x, y);
                if (gameBoard->isValidLocation(CheckLocation) &&
                    gameBoard->isSquareOccupied(CheckLocation))
                {
                    APiece* OccupyingPiece = gameBoard->getPieceAtLocation(CheckLocation);
                    if (OccupyingPiece && OccupyingPiece->GetClass()->IsChildOf(APiece::StaticClass()))
                    {
                        APiece* AllyPiece = Cast<APiece>(OccupyingPiece);
                        if (AllyPiece && AllyPiece->getPieceColor() == this->pieceColor) // Ensure it's an ally
                        {
                            for (const EPieceDirection& Direction : TeleportDirections)
                            {
                                TArray<FVector2D> LineMoves = getLineMoveWithNoObstacle(CheckLocation, Direction, movePoint);
                                PossibleMoves.Append(LineMoves);
                                specialPossibleMove.Append(LineMoves);
                            }
                        }
                    }
                }
            }
        }
    }

    return PossibleMoves;
}

void APieceGuard::bePlacedSpecialSquareEffect_Implementation(AEnvSquare* squareDestination)
{
    moveMode = EPieceMoveMode::ETeleport;
}
