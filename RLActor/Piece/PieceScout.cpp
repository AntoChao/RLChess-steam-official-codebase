#include "PieceScout.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvBoard.h"

APieceScout::APieceScout()
{
}

TArray<FVector2D> APieceScout::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the queen is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* GameBoard = UMapManager::get()->getGameBoard();
    if (GameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();

        // Directions queen can move: all four diagonal directions plus the four cardinal directions
        TArray<EPieceDirection> Directions = {
            EPieceDirection::EUp, EPieceDirection::EDown,
            EPieceDirection::ELeft, EPieceDirection::ERight,
            EPieceDirection::EUpRight, EPieceDirection::EUpLeft,
            EPieceDirection::EDownRight, EPieceDirection::EDownLeft
        };

        // Iterate over each direction and calculate possible moves
        for (EPieceDirection Direction : Directions)
        {
            TArray<FVector2D> LineMoves = getLineMoveWithFirstObstacle(CurrentLocation, Direction, movePoint);
            PossibleMoves.Append(LineMoves);
        }
    }

    return PossibleMoves;
}

void APieceScout::inBenchInteractedEffect(APlayerCharacter* Sender)
{
    UMapManager* mapManager = UMapManager::get();
    AEnvBoard* gameBoard = mapManager->getGameBoard();

    if (gameBoard)
    {
        gameBoard->setAllUnoccupiedColor(pieceColor);
    }
}

void APieceScout::bePlacedInBenchEffect_Implementation(AEnvSquare* squareDestination)
{
    // enable swap position
    if (squareDestination->getIsOccupied())
    {
        if (curSquare->getSquareColorField() == pieceColor && 
            squareDestination->getSquareColorField() == pieceColor)
        {
            swapLocation(squareDestination);
        }
    }
    else
    {
        // setPieceStatus(EPieceStatus::EInBoard);
        if (IsValid(curSquare))
        {
            curSquare->occupiedPieceLeaved();
        }

        // teleport to squareLocation
        curSquare = squareDestination;
        curSquare->beOccupied(this);
        SetActorLocation(curSquare->getPlacementLocation());
    }
}