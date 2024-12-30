// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnvBoard.h"
#include "../Factory/FactoryEnvironment.h"
#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "EnvSquare.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/PlayerCharacter.h"

#include "../Piece/Piece.h"

#include "Net/UnrealNetwork.h"

AEnvBoard::AEnvBoard() 
{
    bReplicates = true;
}

void AEnvBoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AEnvBoard, allSquares);
}

void AEnvBoard::initialized_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Board: Been initialized"));

    totalSquareNum = rowSize * columnSize;
    allSquares.SetNum(totalSquareNum);

    // Use integer calculations for centerLocation
    centerLocation = FVector2D(rowSize / 2.0f, columnSize / 2.0f);
    
    AGameplayGameMode* gameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());
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
}

void AEnvBoard::initializeBoardColor_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Board: Initialed board color"));

    UWorld* serverWorld = GetWorld();

    AGameplayGameMode* gameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());

    if (gameMode && serverWorld)
    {
        ARLGameState* serverGameState = Cast<ARLGameState>(serverWorld->GetGameState());
        UFactoryEnvironment* squareFactory = gameMode->environmentFactoryInstance;

        if (serverGameState && squareFactory)
        {
            TArray<APlayerCharacter*> allPlayers = serverGameState->getAllPlayers();
            bool isPlayerZone;

            // Iterate over the board and assign colorField based on player sections
            for (int Y = 0; Y < columnSize; ++Y)
            {
                for (int X = 0; X < rowSize; ++X)
                {
                    isPlayerZone = false;
                    int Index = getIndexFromLocation(FVector2D(Y, X));
                    if (!allSquares.IsValidIndex(Index)) continue;  // Safety check

                    AEnvSquare* curSquare = allSquares[Index];
                    if (!curSquare) continue;  // Check for null pointers

                    // Player 0: top centre rows
                    if (Y < 2 && X >= 2 && X <= rowSize - 3 && allPlayers.IsValidIndex(0))
                    {
                        allSquares[Index]->setSquareColorField(allPlayers[0]->getPlayerColor());
                        isPlayerZone = true;
                    }
                    // Player 1: bottom centre rows
                    else if (Y >= columnSize - 2 && X >= 2 && X <= rowSize - 3 && allPlayers.IsValidIndex(1))
                    {
                        allSquares[Index]->setSquareColorField(allPlayers[1]->getPlayerColor());
                        isPlayerZone = true;
                    }
                    // Player 2: left centre columns
                    else if (X < 2 && Y >= 2 && Y <= columnSize - 3 && allPlayers.IsValidIndex(2))
                    {
                        allSquares[Index]->setSquareColorField(allPlayers[2]->getPlayerColor());
                        isPlayerZone = true;
                    }
                    // Player 3: right centre columns
                    else if (X >= rowSize - 2 && Y >= 2 && Y <= columnSize - 3 && allPlayers.IsValidIndex(3))
                    {
                        allSquares[Index]->setSquareColorField(allPlayers[3]->getPlayerColor());
                        isPlayerZone = true;
                    }

                    if (isPlayerZone)
                    {
                        // spawn the invinsible wall
                        FVector locationToSpawn = allSquares[Index]->GetActorLocation() + FVector(0.0f, 0.0f, squareLength);
                        AEnvSquare* newInvSquare = Cast<AEnvSquare>(squareFactory->createRLActor(TEXT("InvisibleSquare"), locationToSpawn, squareRotation));
                    }
                }
            }
        }
    }
}

FVector AEnvBoard::getSpawnStartPositionForPlayer(int playerIndex)
{
    switch (playerIndex)
    {
    case 0: // Player 1: top of center
        return FVector(boardCenter.X - squareLength, boardCenter.Y, 0.0f);
    case 1: // Player 2: button of center
        return FVector(boardCenter.X + squareLength, boardCenter.Y, 0.0f);
    case 2: // Player 3: Left of Center
        return FVector(boardCenter.X, boardCenter.Y - squareLength, 0.0f);
    case 3: // Player 4: Right of Center
        return FVector(boardCenter.X, boardCenter.Y + squareLength, 0.0f);
    default:
        return FVector::ZeroVector; // Fallback for an undefined player index
    }
}

FRotator AEnvBoard::getSpawnStartRotationForPlayer(int playerIndex)
{
    FVector DirectionToCenter = boardCenter - getSpawnStartPositionForPlayer(playerIndex);
    FRotator playerDirection = DirectionToCenter.GetSafeNormal().Rotation();
    return playerDirection;
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
    if (initLocation.X < 2)
    {
        return EPieceDirection::EDown;
    }
    if (initLocation.X >= (rowSize - 2))
    {
        return EPieceDirection::EUp;
    }
    if (initLocation.Y < 2)
    {
        return EPieceDirection::ERight;
    }
    if (initLocation.Y >= (rowSize - 2))
    {
        return EPieceDirection::ELeft;
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

FString AEnvBoard::GetActorName(ELanguage curLanguage)
{
    return TEXT("Board");
}

FString AEnvBoard::GetDescription(ELanguage curLanguage)
{
    return TEXT("A Board");
}

FString AEnvBoard::GetInteractionDescription(ELanguage curLanguage)
{
    return TEXT("Interact with Board");
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

void AEnvBoard::setSpecificColor_Implementation(FColor aColor) // server
{
    for (AEnvSquare* aSquare : allSquares)
    {
        if (aSquare && aSquare->getSquareColorField() == aColor)
        {
            aSquare->setIsPossibleMove(true, aColor); // client
        }
    }
}

void AEnvBoard::setAllUnoccupiedColor(FColor aColor)
{
    for (AEnvSquare* aSquare : allSquares)
    {
        if (aSquare && !aSquare->getIsOccupied())
        {
            if (aSquare->getSquareColorField() == aColor || aSquare->getSquareColorField() == FColor::White)
            {
                aSquare->setIsPossibleMove(true, aColor);
            }
        }
    }
}

void AEnvBoard::setPossibleMoves(APiece* onePiece) //non rpc
{
    TArray<FVector2D> allPossibles = onePiece->calculatePossibleMove();
    FColor pieceColor = onePiece->getPieceColor();
    for (FVector2D eachLocation : allPossibles)
    {
        if (isValidLocation(eachLocation))
        {
            int index = getIndexFromLocation(eachLocation);
            if (allSquares[index])
            {
                allSquares[index]->setIsPossibleMove(true, pieceColor); // client
                allSquares[index]->setPreviewMesh(onePiece); // client
            }
        }
    }
}

void AEnvBoard::resetBoard() // Client
{
    for (AEnvSquare* aSquare : allSquares)
    {
        if (IsValid(aSquare))
        {
            aSquare->setIsPossibleMove(false, FColor::Transparent); // client
            aSquare->setPreviewMesh(nullptr); // client
        }
    }
}
void AEnvBoard::resetConfirmedMeshBoard()
{
    for (AEnvSquare* aSquare : allSquares)
    {
        if (IsValid(aSquare))
        {
            aSquare->cancelConfirmedMeshMulticast();
        }
    }
}

bool AEnvBoard::isSquareOccupied(FVector2D aLocation)
{
    AEnvSquare* targetSquare = allSquares[getIndexFromLocation(aLocation)];
    return targetSquare->getIsOccupied();
}

bool AEnvBoard::isPlayerOnTop(FVector2D aLocation)
{
    AEnvSquare* targetSquare = allSquares[getIndexFromLocation(aLocation)];
    return targetSquare->getIsPlayerOnTop();
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