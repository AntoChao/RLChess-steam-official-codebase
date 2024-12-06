// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnvBoard.h"
#include "../Factory/FactoryEnvironment.h"
#include "../../RLHighLevel/GameplayGameMode.h"
#include "EnvSquare.h"
#include "../Player/PlayerCharacter.h"

AEnvBoard::AEnvBoard() {
    
}

void AEnvBoard::initialized(TArray<APlayerCharacter*> allPlayers)
{
    totalSquareNum = rowSize * columnSize;
    allSquares.SetNum(totalSquareNum);

    // Use integer calculations for centerLocation
    centerLocation = FVector2D(rowSize / 2.0f, columnSize / 2.0f);

    AGameplayGameMode* gameMode = Cast<AGameplayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    UFactoryEnvironment* squareFactory = gameMode->environmentFactoryInstance;

    if (squareFactory)
    {
        // Calculate board dimensions
        float boardWidth = rowSize * squareLength;
        float boardHeight = columnSize * squareLength;

        // Calculate top-left corner of the board
        FVector topLeft = boardCenter - FVector(FMath::RoundToFloat(boardWidth / 2.0f), FMath::RoundToFloat(boardHeight / 2.0f), 0.0f);

        for (int row = 0; row < rowSize; ++row)
        {
            for (int col = 0; col < columnSize; ++col)
            {
                // Calculate position of the current square
                float x = topLeft.X + (col * squareLength) + (squareLength / 2.0f);
                float y = topLeft.Y + (row * squareLength) + (squareLength / 2.0f);
                float z = boardCenter.Z; // Use consistent Z-level for all squares

                // Round coordinates to prevent precision errors
                x = FMath::RoundToFloat(x);
                y = FMath::RoundToFloat(y);

                FVector newSquarePosition(x, y, z);

                AEnvSquare* newSquare = nullptr;

                // Determine the square type (Black or White) based on row and column
                FString squareType = ((row + col) % 2 == 0) ? TEXT("BlackSquare") : TEXT("WhiteSquare");
                newSquare = Cast<AEnvSquare>(squareFactory->createRLActor(squareType, newSquarePosition, squareRotation));

                if (newSquare)
                {
                    // Set the square's world location and grid location
                    newSquare->SetActorLocation(newSquarePosition);
                    newSquare->setSquareLocation(FVector2D(row, col));

                    // Use integer indexing for the array
                    int squareIndex = getIndexFromLocation(FVector2D(row, col));
                    if (squareIndex >= 0 && squareIndex < allSquares.Num())
                    {
                        allSquares[squareIndex] = newSquare;
                    }
                }
            }
        }
    }

    initializeBoardColor(allPlayers);
    resetBoard();
}

void AEnvBoard::initializeBoardColor(TArray<APlayerCharacter*> allPlayers)
{
    // Iterate over the board and assign colorField based on player sections
    for (int Y = 0; Y < columnSize; ++Y)
    {
        for (int X = 0; X < rowSize; ++X)
        {
            int Index = getIndexFromLocation(FVector2D(Y, X));
            if (!allSquares.IsValidIndex(Index)) continue;  // Safety check

            AEnvSquare* curSquare = allSquares[Index];
            if (!curSquare) continue;  // Check for null pointers

            // Player 0: top centre rows
            if (Y < 2 && X > 2 && X < rowSize - 3 && allPlayers.IsValidIndex(0))
            {
                curSquare->setSquareColorField(allPlayers[0]->getPlayerColor());
            }
            // Player 1: bottom centre rows
            else if (Y >= columnSize - 2 && X > 2 && X < rowSize - 3 && allPlayers.IsValidIndex(1))
            {
                curSquare->setSquareColorField(allPlayers[1]->getPlayerColor());
            }
            // Player 2: left centre columns
            else if (X < 2 && Y > 2 && Y < columnSize - 3 && allPlayers.IsValidIndex(2))
            {
                curSquare->setSquareColorField(allPlayers[2]->getPlayerColor());
            }
            // Player 3: right centre columns
            else if (X >= rowSize - 2 && Y > 2 && Y < columnSize - 3 && allPlayers.IsValidIndex(3))
            {
                curSquare->setSquareColorField(allPlayers[3]->getPlayerColor());
            }
        }
    }
}

FVector AEnvBoard::getSpawnStartPositionForPlayer(int playerIndex)
{
    int spawnRowSize = rowSize - 1;
    int spawnColumnSize = columnSize - 1;
    FVector topLeft = boardCenter - FVector((spawnRowSize * squareLength) / 2.0f, (spawnColumnSize * squareLength) / 2.0f, 0.0f);
    FVector bottomRight = boardCenter + FVector((spawnRowSize * squareLength) / 2.0f, (spawnColumnSize * squareLength) / 2.0f, 0.0f);

    switch (playerIndex)
    {
    case 0: // Player 1: Top-center of the first row
        return FVector(topLeft.X + (spawnRowSize * squareLength / 2), topLeft.Y, 0.0f);
    case 1: // Player 2: Bottom-center of the last row
        return FVector(topLeft.X + (spawnRowSize * squareLength / 2), topLeft.Y + (spawnColumnSize * squareLength), 0.0f);
    case 2: // Player 3: Center of the first column
        return FVector(topLeft.X, topLeft.Y + (spawnColumnSize * squareLength / 2), 0.0f);
    case 3: // Player 4: Center of the last column
        return FVector(topLeft.X + (spawnRowSize * squareLength), topLeft.Y + (spawnColumnSize * squareLength / 2), 0.0f);
    default:
        return FVector::ZeroVector; // Fallback for an undefined player index
    }
}

FRotator AEnvBoard::getSpawnStartRotationForPlayer(int playerIndex)
{
    FVector DirectionToCenter = boardCenter - getSpawnStartPositionForPlayer(playerIndex);
    DirectionToCenter.Z = 0;  // Ensure the direction vector is purely horizontal
    float YawAngle = FMath::Atan2(DirectionToCenter.Y, DirectionToCenter.X) * (180.f / PI);

    // FRotator(Pitch, Yaw, Roll)
    return FRotator(0.0f, YawAngle, 0.0f);
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

EPieceDirection AEnvBoard::calculateInitDirection(FVector2D initLocation)
{
    // Calculate the direction vector
    FVector2D directionVector = centerLocation - initLocation;

    // Normalize the vector to find the dominant direction
    directionVector.Normalize();

    // Determine the direction based on the vector components
    if (FMath::Abs(directionVector.X) > FMath::Abs(directionVector.Y))
    {
        // Horizontal dominance
        if (directionVector.X > 0)
        {
            return EPieceDirection::ERight;
        }
        else
        {
            return EPieceDirection::ELeft;
        }
    }
    else
    {
        // Vertical dominance
        if (directionVector.Y > 0)
        {
            return EPieceDirection::EUp;
        }
        else
        {
            return EPieceDirection::EDown;
        }
    }

    return EPieceDirection::ENone; // Fallback (e.g., when InitPoint == CenterLocation)
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

    if (x < 0 || x >= rowSize)
    {
        return false;
    }

    if (y < 0 || y >= columnSize)
    {
        return false;
    }
    
    return true;
}

AEnvSquare* AEnvBoard::getSquareAtLocation(FVector2D aLocation)
{
    return allSquares[getIndexFromLocation(aLocation)];
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

int AEnvBoard::getRowSize()
{
    return rowSize;
}
int AEnvBoard::getColumnSize()
{
    return columnSize;
}
int AEnvBoard::getSquareLength()
{
    return squareLength;
}

void AEnvBoard::setSpecificColor(FColor aColor)
{
    for (AEnvSquare* aSquare : allSquares)
    {
        if (aSquare->getSquareColorField() == aColor)
        {
            aSquare->setIsPossibleMove(true, aColor);
        }
    }
}

void AEnvBoard::setAllUnoccupiedColor(FColor aColor)
{
    for (AEnvSquare* aSquare : allSquares)
    {
        if (aSquare->getSquareColorField() == aColor && !aSquare->getIsOccupied())
        {
            aSquare->setIsPossibleMove(true, aColor);
        }
    }
}

void AEnvBoard::setPossibleMoves(TArray<FVector2D> allPossibles, FColor pieceColor)
{
    for (FVector2D eachLocation : allPossibles)
    {
        if (isValidLocation(eachLocation))
        {
            int index = getIndexFromLocation(eachLocation);
            allSquares[index]->setIsPossibleMove(true, pieceColor);
        }
    }
}

void AEnvBoard::resetBoard()
{
    for (AEnvSquare* aSquare : allSquares)
    {
        if (IsValid(aSquare))
        {
            aSquare->setIsPossibleMove(false, FColor::Transparent);
        }
    }
}

bool AEnvBoard::isSquareOccupied(FVector2D aLocation)
{
    AEnvSquare* targetSquare = allSquares[getIndexFromLocation(aLocation)];
    return targetSquare->getIsOccupied();
}

APiece* AEnvBoard::getPieceAtLocation(FVector2D aLocation)
{
    AEnvSquare* targetSquare = allSquares[getIndexFromLocation(aLocation)];
    if (targetSquare)
    {
        return targetSquare->getOccupiedPiece();
    }

    return nullptr;
}

TArray<AEnvSquare*> AEnvBoard::getAllSquaresOfSpecificColor(FColor aColor)
{
    TArray<AEnvSquare*> squareOfColor;
    for (AEnvSquare* eachSquare : allSquares)
    {
        if (eachSquare->getSquareColorField() == aColor)
        {
            squareOfColor.Add(eachSquare);
        }
    }

    return squareOfColor;
}