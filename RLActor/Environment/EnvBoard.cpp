// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnvBoard.h"
#include "../Factory/FactoryEnvironment.h"
#include "EnvSquare.h"
#include "../Player/PlayerCharacter.h"

AEnvBoard::AEnvBoard() {
    boardInstance = nullptr;
}

AEnvBoard* AEnvBoard::get()
{
    if (!boardInstance)
    {
        initialize();
    }
    return boardInstance;
}

void AEnvBoard::initialize()
{
    boardInstance = NewObject<AEnvBoard>(boardClass);
    if (boardInstance)
    {
        boardInstance->AddToRoot();
    }
}

void AEnvBoard::createBoard()
{
    totalSquareNum = rowSize * columnSize;
    allSquares.SetNum(totalSquareNum);

    URLFactory* squareFactory = UFactoryEnvironment::get();

    float boardWidth = rowSize * squareLength;
    float boardHeight = columnSize * squareLength;
    FVector topLeft = boardCenter - FVector(boardWidth / 2.0f, boardHeight / 2.0f, 0.0f);
    for (int row = 0; row < rowSize; ++row)
    {
        for (int col = 0; col < columnSize; ++col)
        {
            // Calculate the position of the current square
            float x = topLeft.X + (row * squareLength) + (squareLength / 2.0f);
            float y = topLeft.Y + (col * squareLength) + (squareLength / 2.0f);
            float z = boardCenter.Z; // Use the board's Z-level for all squares

            FVector newSquarePosition(x, y, z);
            AEnvSquare* newSquare = Cast<AEnvSquare>(squareFactory->createRLActor(TEXT("basicSquare"), newSquarePosition, squareRotation));
            newSquare->SetActorLocation(newSquarePosition);
            newSquare->setSquareLocation(FVector2D(row, col));
        }
    }
}

void AEnvBoard::initializeBoardColor(TArray<APlayerCharacter*> allPlayers)
{
    if (allPlayers.Num() < 2 || allPlayers.Num() > 4)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid number of players! Must be between 2 and 4."));
        return;
    }

    // Iterate over the board and assign colorField based on player sections
    for (int Y = 0; Y < columnSize; ++Y)
    {
        for (int X = 0; X < rowSize; ++X)
        {
            int Index = getIndexFromLocation(FVector2D(X, Y));
            // if (!allSquares.IsValidIndex(Index)) continue;

            AEnvSquare* curSquare = allSquares[Index];

            if (curSquare != nullptr) continue;

            if (Y < 2 && X > 1 && X < rowSize - 2 && allPlayers.IsValidIndex(0))
            {
                curSquare->setSquareColorField(allPlayers[0]->getPlayerColor());
            }
            else if (Y >= columnSize - 2 && X > 1 && X < rowSize - 2 && allPlayers.IsValidIndex(1))
            {
                curSquare->setSquareColorField(allPlayers[1]->getPlayerColor());
            }
            else if (X < 2 && Y > 1 && Y < columnSize - 2 && allPlayers.IsValidIndex(2))
            {
                curSquare->setSquareColorField(allPlayers[2]->getPlayerColor());
            }
            else if (X >= rowSize - 2 && Y > 1 && Y < columnSize - 2 && allPlayers.IsValidIndex(3))
            {
                curSquare->setSquareColorField(allPlayers[3]->getPlayerColor());
            }
        }
    }
}

void AEnvBoard::setUpPlayerBench(TArray<APlayerCharacter*> allPlayers)
{
    if (allPlayers.Num() < 2 || allPlayers.Num() > 4)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid number of players! Must be between 2 and 4."));
        return;
    }

    for (int32 playerIndex = 0; playerIndex < allPlayers.Num(); ++playerIndex)
    {
        APlayerCharacter* player = allPlayers[playerIndex];
        if (!player) continue;

        FVector startPosition = getSpawnStartPositionForPlayer(playerIndex);
        FVector placementOffset = getPlayerPlacementOffset(playerIndex);
    }
}

FVector AEnvBoard::getSpawnStartPositionForPlayer(int playerIndex)
{
    FVector topLeft = boardCenter - FVector((rowSize * squareLength) / 2.0f, (columnSize * squareLength) / 2.0f, 0.0f);
    FVector bottomRight = boardCenter + FVector((rowSize * squareLength) / 2.0f, (columnSize * squareLength) / 2.0f, 0.0f);

    switch (playerIndex)
    {
    case 0: // Player 1: Top-left corner, offset outside the board
    {
        return topLeft - FVector(2 * squareLength, 0.0f, 0.0f) + FVector(0.0f, squareLength, 0.0f);
    }
    case 1: // Player 2: Bottom-left corner, offset outside the board
    {
        return topLeft + FVector((columnSize - 2) * squareLength, -2 * squareLength, 0.0f);
    }
    case 2: // Player 3: Left side, offset outside the board
    {
        return topLeft - FVector(squareLength, 2 * squareLength, 0.0f);
    }
    case 3: // Player 4: Right side, offset outside the board
    {
        return bottomRight + FVector(-2 * squareLength, -squareLength, 0.0f);
    }
    default:
    {

        return FVector::ZeroVector;
    }
    }
}

FVector AEnvBoard::getPlayerPlacementOffset(int playerIndex)
{
    // Determine placement direction based on the player's section
    FVector placementOffset;
    switch (playerIndex)
    {
    case 0: // Player 1: Shift to the right
        placementOffset = FVector(0.0f, pieceDistance, 0.0f);
        break;
    case 1: // Player 2: Shift to the left
        placementOffset = FVector(0.0f, -pieceDistance, 0.0f);
        break;
    case 2: // Player 3: Shift downwards
        placementOffset = FVector(pieceDistance, 0.0f, 0.0f);
        break;
    case 3: // Player 4: Shift upwards
        placementOffset = FVector(-pieceDistance, 0.0f, 0.0f);
        break;
    default:
        placementOffset = FVector::ZeroVector;
        break;
    }
    return placementOffset;
}

FVector2D AEnvBoard::getLocationFromIndex(int aIndex)
{
    float x = aIndex % rowSize;
    float y = aIndex / columnSize;
    FVector2D value = FVector2D(x, y);
    return value;
}
int AEnvBoard::getIndexFromLocation(FVector2D aLocation)
{
    return aLocation.Y * rowSize + aLocation.X;
}

bool AEnvBoard::isValidLocation(FVector2D aLocation)
{
    int x = aLocation.X;
    int y = aLocation.Y;

    if (x < 0 && x >= rowSize)
    {
        return false;
    }

    if (y < 0 && y>= columnSize)
    {
        return false;
    }
    
    return true;
}

FString AEnvBoard::GetActorName()
{
    return TEXT("Board");
}

FString AEnvBoard::GetDescription()
{
    return TEXT("A Board");
}

bool AEnvBoard::IsAbleToBeInteracted(APlayerCharacter* Sender)
{
    return false;
}

void AEnvBoard::BeInteracted(APlayerCharacter* Sender)
{
    return;
}

void AEnvBoard::BeUnInteracted(APlayerCharacter* Sender)
{
    return;
}

void AEnvBoard::setSpecificColor(FColor aColor)
{
    for (AEnvSquare* aSquare : allSquares)
    {
        if (aSquare->getSquareColorField() == aColor)
        {
            aSquare->setIsPossibleMove(true);
        }
    }
}

void AEnvBoard::setPossibleMoves(TArray<FVector2D> allPossibles)
{
    for (FVector2D eachLocation : allPossibles)
    {
        if (isValidLocation(eachLocation))
        {
            int index = getIndexFromLocation(eachLocation);
            allSquares[index]->setIsPossibleMove(true);
        }
    }
}

void AEnvBoard::resetBoard()
{
    for (AEnvSquare* aSquare : allSquares)
    {
        if (IsValid(aSquare))
        {
            aSquare->setIsPossibleMove(false);
        }
    }
}