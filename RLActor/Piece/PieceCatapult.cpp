#include "PieceCatapult.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "../Environment/EnvBoard.h"

APieceCatapult::APieceCatapult()
{
}

TArray<FVector2D> APieceCatapult::calculatePossibleMove()
{
    launchPiece = nullptr;
    TArray<FVector2D> PossibleMoves;

    allValidAdjacentSquares.Empty();
    allValidAdjacentPieces.Empty();

    if (!curSquare) // Ensure the catapult is on the board
    {
        return PossibleMoves;
    }

    AEnvBoard* GameBoard = nullptr;
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            GameBoard = GameState->getGameBoard();
        }
    }

    if (GameBoard)
    {
        FVector2D CurrentLocation = curSquare->getSquareLocation();

        TArray<FVector2D> AdjacentDirections = {
            FVector2D(-1, 0), FVector2D(1, 0),  // Up, Down
            FVector2D(0, -1), FVector2D(0, 1)  // Left, Right
        };

        for (const FVector2D& Direction : AdjacentDirections)
        {
            TArray<FVector2D> LineMoves = getLineMoveWithFirstObstacle(CurrentLocation, directionVectorToEnum(Direction), INT_MAX); // INT_MAX to simulate unlimited range
            
            if (LineMoves.Num() == 1)
            {
                AEnvSquare* adjacentSquare = GameBoard->getSquareAtLocation(LineMoves[0]);

                EPieceDirection aDirection = directionVectorToEnum(Direction);
                TArray<FVector2D> allLaunchablePlaces = getLineMove(CurrentLocation, aDirection, INT_MAX);

                allValidAdjacentSquares.Add(adjacentSquare);
                allValidAdjacentPieces.Add(adjacentSquare->getOccupiedPiece());
                PossibleMoves.Append(allLaunchablePlaces);
            }
        }

    }

    return PossibleMoves;
}

void APieceCatapult::bePlacedInBoardEffect_Implementation(AEnvSquare* squareDestination)
{
    if (squareDestination)
    {
        for (APiece* adjPiece : allValidAdjacentPieces)
        {
            if (adjPiece && adjPiece->getOccupiedSquare())
            {
                AEnvSquare* aSquare = adjPiece->getOccupiedSquare();

                FVector2D aLocation = aSquare->getSquareLocation();
                FVector2D anotherLocation = squareDestination->getSquareLocation();

                if (aLocation.X == anotherLocation.X || aLocation.Y == anotherLocation.Y)
                {
                    adjPiece->beLaunchedTo(squareDestination);
                }
            }
        }
    }
}