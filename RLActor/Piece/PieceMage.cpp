#include "PieceMage.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Environment/EnvBoard.h"

APieceMage::APieceMage()
{
}

TArray<FVector2D> APieceMage::calculatePossibleMove()
{
    TArray<FVector2D> PossibleMoves;

    if (!curSquare) // Ensure the mage is on the board
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
        for (int x = 0; x < gameBoard->getRowSize(); ++x)
        {
            for (int y = 0; y < gameBoard->getColumnSize(); ++y)
            {
                FVector2D CheckPosition = FVector2D(x, y);
                // Check if the position is occupied and not the current position of the mage
                if (gameBoard->isSquareOccupied(CheckPosition) && CheckPosition != curSquare->getSquareLocation())
                {
                    PossibleMoves.Add(CheckPosition);
                }
            }
        }
    }

    return PossibleMoves;
}

void APieceMage::endMoving_Implementation()
{
    swapLocation(targetSquare);

    isMoving = false;
    /*
    curSquare = targetSquare;
    // teleport to squareLocation
    curSquare->beOccupied(this);*/

    if (isKilledAnyActorThisTurn)
    {
        killEffect();
    }
}