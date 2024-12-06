// Copyright Epic Games, Inc. All Rights Reserved.

#include "Piece.h"

#include "../../RLHighLevel/GameplayGameMode.h"

#include "../Environment/EnvShop.h"
#include "../Player/PlayerCharacter.h"
#include "../Environment/EnvSquare.h"
#include "../Environment/EnvBoard.h"

APiece::APiece()
{
    // Create and set the root component as a SceneComponent
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(Root);

    // Create the StaticMeshComponent and attach it to the root
    pieceBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
    pieceBody->SetupAttachment(RootComponent);

    // Create the BoxComponent and attach it to the root
    pieceCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    pieceCollision->SetupAttachment(RootComponent);

    // Configure Collision Settings
    pieceCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    pieceCollision->SetCollisionObjectType(ECC_WorldDynamic);
    pieceCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
    pieceCollision->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

    // Movement
    // movementTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MovementTimeline"));

    PrimaryActorTick.bCanEverTick = true;
}

void APiece::BeginPlay()
{
    Super::BeginPlay();

    // Bind Overlap Event
    if (pieceCollision)
    {
        pieceCollision->OnComponentBeginOverlap.AddDynamic(this, &APiece::OnOverlapBegin);
    }

    initializeMaterials();
    // setPieceColor(FColor::Silver);
    
}

void APiece::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // movementTimeline.TickTimeline(DeltaTime);
}

FString APiece::GetActorName()
{
	return pieceName;
}

FString APiece::GetDescription()
{
	return pieceDescription;
}

bool APiece::IsAbleToBeInteracted(APlayerCharacter* Sender)
{
    if (pieceStatus == EPieceStatus::EInShop)
    {
        return true;
    }
    else
    {
        return Sender->getPlayerColor() == pieceColor;
    }
	
}

void APiece::BeInteracted(APlayerCharacter* Sender)
{
    switch (pieceStatus)
    {
        case EPieceStatus::EInShop:
        {
            inShopInteractedEffect(Sender);
            break;
        }
        case EPieceStatus::EInBench:
        {
            inBenchInteractedEffect(Sender);
            break;
        }
        case EPieceStatus::EInBoard:
        {
            inBoardInteractedEffect(Sender);
            break;
        }
        default:
            break;
    }
}

void APiece::inShopInteractedEffect(APlayerCharacter* Sender)
{
    UMapManager* mapManager = UMapManager::get();
    AEnvShop* gameShop = mapManager->getShop();
    if (gameShop)
    {
        TScriptInterface<IRLProduct> pieceAsProduct;
        pieceAsProduct.SetObject(this);
        pieceAsProduct.SetInterface(Cast<IRLProduct>(this));

        gameShop->sellProduct(Sender, pieceAsProduct);
    }
}

void APiece::inBenchInteractedEffect(APlayerCharacter* Sender)
{
    UMapManager* mapManager = UMapManager::get();
    AEnvBoard* gameBoard = mapManager->getGameBoard();
    if (gameBoard)
    {
        gameBoard->setSpecificColor(pieceColor);
    }

    inBenchSpecialEffect();
}

void APiece::inBenchSpecialEffect()
{
    return;
}


void APiece::inBoardInteractedEffect(APlayerCharacter* Sender)
{
    UMapManager* mapManager = UMapManager::get();
    AEnvBoard* gameBoard = mapManager->getGameBoard();

    TArray<FVector2D> allPossibles = calculatePossibleMove();

    if (gameBoard)
    {
        gameBoard->setPossibleMoves(allPossibles, pieceColor);
    }
}


void APiece::BeUnInteracted(APlayerCharacter* Sender)
{
    return;
}

int APiece::GetProductCost()
{
    return pieceLevel;
}

FString APiece::GetProductName()
{
    return pieceName;
}

UTexture2D* APiece::GetProductImage()
{
    return nullptr;
}

void APiece::initializeMaterials()
{
    // Associate colors with materials
    colorToMaterial.Add(FColor::Silver, silverMaterial);
    colorToMaterial.Add(FColor::Red, redMaterial);
    colorToMaterial.Add(FColor::Blue, blueMaterial);
    colorToMaterial.Add(FColor::Green, greenMaterial);
    colorToMaterial.Add(FColor::Yellow, yellowMaterial);
    colorToMaterial.Add(FColor::Purple, purpleMaterial);

}

/* piece information*/
void APiece::initializeDirection(AEnvSquare* squareDestination)
{
    AEnvBoard* gameBoard = UMapManager::get()->getGameBoard();
    pieceDirection = gameBoard->calculateInitDirection(squareDestination->getSquareLocation());
}

TArray<FVector2D> APiece::calculatePossibleMove()
{
    TArray<FVector2D> emptyArray;
    return emptyArray;
}


// common move functions
FVector2D APiece::getDirectionVector(EPieceDirection Direction) const
{
    switch (Direction)
    {
    case EPieceDirection::EUp:
        return FVector2D(0, 1);
    case EPieceDirection::EDown:
        return FVector2D(0, -1);
    case EPieceDirection::ELeft:
        return FVector2D(-1, 0);
    case EPieceDirection::ERight:
        return FVector2D(1, 0);
    case EPieceDirection::EUpLeft:
        return FVector2D(-1, 1);
    case EPieceDirection::EUpRight:
        return FVector2D(1, 1);
    case EPieceDirection::EDownLeft:
        return FVector2D(-1, -1);
    case EPieceDirection::EDownRight:
        return FVector2D(1, -1);
    default:
        return FVector2D(0, 0); // Should ideally never be hit
    }
}

EPieceDirection APiece::directionVectorToEnum(FVector2D DirectionVector) const
{
    // Mapping direction vectors to the corresponding enum values for all eight directions
    if (DirectionVector == FVector2D(0, 1)) return EPieceDirection::EUp;
    if (DirectionVector == FVector2D(0, -1)) return EPieceDirection::EDown;
    if (DirectionVector == FVector2D(-1, 0)) return EPieceDirection::ELeft;
    if (DirectionVector == FVector2D(1, 0)) return EPieceDirection::ERight;
    if (DirectionVector == FVector2D(-1, 1)) return EPieceDirection::EUpLeft;
    if (DirectionVector == FVector2D(1, 1)) return EPieceDirection::EUpRight;
    if (DirectionVector == FVector2D(-1, -1)) return EPieceDirection::EDownLeft;
    if (DirectionVector == FVector2D(1, -1)) return EPieceDirection::EDownRight;
    return EPieceDirection::ENone; // Return a default case if no direction matches
}

TArray<FVector2D> APiece::getDiagonalDirections() const
{
    TArray<FVector2D> DiagonalDirections;
    // Add diagonal vectors depending on the piece's direction
    switch (pieceDirection)
    {
    case EPieceDirection::EUp:
        DiagonalDirections.Add(FVector2D(-1, 1));  // Top-left
        DiagonalDirections.Add(FVector2D(1, 1));   // Top-right
        break;
    case EPieceDirection::EDown:
        DiagonalDirections.Add(FVector2D(-1, -1)); // Bottom-left
        DiagonalDirections.Add(FVector2D(1, -1));  // Bottom-right
        break;
    case EPieceDirection::ELeft: // Not typically used but included for completeness
        DiagonalDirections.Add(FVector2D(-1, -1)); // Left-bottom
        DiagonalDirections.Add(FVector2D(-1, 1));  // Left-top
        break;
    case EPieceDirection::ERight: // Not typically used but included for completeness
        DiagonalDirections.Add(FVector2D(1, -1));  // Right-bottom
        DiagonalDirections.Add(FVector2D(1, 1));   // Right-top
        break;
    default:
        break;
    }

    return DiagonalDirections;
}


TArray<FVector2D> APiece::getLineMove(FVector2D CurrentLocation, EPieceDirection Direction, int Distance)
{
    TArray<FVector2D> LineMoves;

    // Convert the direction enum to a vector
    FVector2D DirectionVector = getDirectionVector(Direction);
    if (DirectionVector == FVector2D(0, 0))
    {
        return LineMoves; // Return empty array if direction is invalid
    }

    AEnvBoard* gameBoard = UMapManager::get()->getGameBoard();
    if (!gameBoard)
    {
        return LineMoves; // Return empty array if board is not found
    }

    for (int Step = 1; Step <= Distance; ++Step)
    {
        FVector2D NextLocation = CurrentLocation + DirectionVector * Step;

        // Use your existing function to check if the NextLocation is valid
        if (!gameBoard->isValidLocation(NextLocation))
        {
            break; // Stop calculating further if it goes outside the board or is invalid
        }

        LineMoves.Add(NextLocation);
    }

    return LineMoves;
}

TArray<FVector2D> APiece::getLineMoveWithFirstObstacle(FVector2D CurrentLocation, EPieceDirection Direction, int Distance)
{
    TArray<FVector2D> LineMoves;

    // Use the base function to get potential moves without considering collisions
    TArray<FVector2D> PotentialMoves = getLineMove(CurrentLocation, Direction, Distance);

    AEnvBoard* gameBoard = UMapManager::get()->getGameBoard();
    if (!gameBoard)
    {
        return LineMoves; // Return empty array if board is not found
    }

    for (FVector2D Move : PotentialMoves)
    {
        if (gameBoard->isSquareOccupied(Move))
        {
            // Include this move as it's a possible capture but stop further moves
            LineMoves.Add(Move);
            break;
        }
        else
        {
            // Add the move as it's unoccupied and valid
            LineMoves.Add(Move);
        }
    }

    return LineMoves;
}

TArray<FVector2D> APiece::getLineMoveWithNoObstacle(FVector2D CurrentLocation, EPieceDirection Direction, int Distance)
{
    TArray<FVector2D> LineMoves;

    // Use the base function to get potential moves without considering collisions
    TArray<FVector2D> PotentialMoves = getLineMove(CurrentLocation, Direction, Distance);

    AEnvBoard* gameBoard = UMapManager::get()->getGameBoard();
    if (!gameBoard)
    {
        return LineMoves; // Return empty array if board is not found
    }

    for (FVector2D Move : PotentialMoves)
    {
        if (gameBoard->isSquareOccupied(Move))
        {
            break;
        }
        else
        {
            // Add the move as it's unoccupied and valid
            LineMoves.Add(Move);
        }
    }

    return LineMoves;
}


void APiece::dieEffect()
{
    return;
}

/* piece collision*/
void APiece::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this)) // Check validity of the other actor
    {
        if (isMoving)
        {
            UE_LOG(LogTemp, Warning, TEXT("%s overlapped with %s"), *GetName(), *OtherActor->GetName());
            if (OtherActor->IsA(APlayerCharacter::StaticClass()))
            {
                APlayerCharacter* playerCollided = Cast<APlayerCharacter>(OtherActor);
                playerCollided->beCollidedByPiece(this);
            }
        }
    }
}

/* piece moving*/
EPieceStatus APiece::getPieceStatus()
{
    return pieceStatus;
}
void APiece::setPieceStatus(EPieceStatus newStatus)
{
    pieceStatus = newStatus;
}

void APiece::setPieceColor(FColor aColor)
{
    pieceColor = aColor;

    if (colorToMaterial.Contains(aColor))
    {
        UMaterialInterface* SelectedMaterial = colorToMaterial[aColor];

        if (SelectedMaterial && pieceBody)
        {
            int32 MaterialCount = pieceBody->GetNumMaterials(); // Get the number of materials on the mesh
            for (int32 Index = 0; Index < MaterialCount; ++Index)
            {
                pieceBody->SetMaterial(Index, SelectedMaterial); // Set the material for each index
            }
        }

        pieceBody->SetVisibility(true); // Ensure the piece is visible after setting the materials
    }
}

FColor APiece::getPieceColor()
{
    return pieceColor;
}

int APiece::getLevel()
{
    return pieceLevel;
}

/* setActor location in a time constraint*/
void APiece::bePlaced(AEnvSquare* squareDestination)
{
    if (pieceStatus == EPieceStatus::EInShop)
    {
        bePlacedInShopEffect(squareDestination);
    }
    else if (pieceStatus == EPieceStatus::EInBench)
    {
        bePlacedInBenchEffect(squareDestination);
    }
    else
    {
        bePlacedInBoardEffect(squareDestination);
    }

    AEnvBoard* gameBoard = UMapManager::get()->getGameBoard();
    if (gameBoard)
    {
        gameBoard->resetBoard();
    }
}

void APiece::bePlacedInShopEffect(AEnvSquare* squareDestination)
{
    setPieceStatus(EPieceStatus::EInBench);

    if (IsValid(curSquare))
    {
        curSquare->occupiedPieceLeaved();
    }

    // teleport to squareLocation
    curSquare = squareDestination;
    initializeDirection(squareDestination); // initialize the direction

    curSquare->beOccupied(this);
    SetActorLocation(curSquare->getPlacementLocation());
}

void APiece::bePlacedInBenchEffect(AEnvSquare* squareDestination)
{
    // enable swap position
    if (squareDestination->getIsOccupied())
    {
        swapLocation(squareDestination);
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

void APiece::swapLocation(AEnvSquare* squareDestination)
{
    if (squareDestination && curSquare)
    {
        APiece* pieceToSwap = squareDestination->getOccupiedPiece();
        if (pieceToSwap)
        {
            if (IsValid(curSquare))
            {
                curSquare->occupiedPieceLeaved();
            } // avoid collision
            SetActorLocation(curSquare->getPlacementLocation() - FVector(0.0f, 0.0f, -1000.0f));
        
            // let the piece occupy this position
            pieceToSwap->bePlaced(curSquare);

            // teleport to squareLocation
            curSquare = squareDestination;
            curSquare->beOccupied(this);
            SetActorLocation(curSquare->getPlacementLocation());
        }
    }
}

// all pieces are inboard when setup time is finished
void APiece::bePlacedInBoardEffect(AEnvSquare* squareDestination)
{
    if (!bHasMoved)
    {
        firstInBoardMovedEffect(squareDestination);
    }
    
    startMoving(squareDestination);
}

void APiece::firstInBoardMovedEffect(AEnvSquare* squareDestination)
{
    bHasMoved = true;
}

void APiece::startMoving(AEnvSquare* squareDestination)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("CHECK"));

    if (IsValid(curSquare))
    {
        curSquare->occupiedPieceLeaved();
    }

    FVector startLocation = GetActorLocation() + FVector(0.0f, 0.0f, 300.0f);
    FVector endLocation = squareDestination->getPlacementLocation() + FVector(0.0f, 0.0f, 300.0f);

    DrawDebugPoint(GetWorld(), startLocation, 10.0f, FColor::Green, false, 2.0f);
    DrawDebugPoint(GetWorld(), endLocation, 10.0f, FColor::Red, false, 2.0f);

    // moveBasedOnMove(squareDestination);
}

void APiece::endMoving(AEnvSquare* squareDestination)
{
    // SetActorLocation(endLocation);

    curSquare = squareDestination;
    // teleport to squareLocation
    curSquare->beOccupied(this);
}