// Copyright Epic Games, Inc. All Rights Reserved.

#include "Piece.h"

#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Curves/CurveFloat.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

#include "../../RLHighLevel/GameplayGameMode.h"
#include "../../RLHighLevel/RLGameState.h"

#include "PiecePreviewMesh.h"
#include "PieceConfirmedMesh.h"
#include "PieceFractureMesh.h"

#include "../Player/PlayerCharacter.h"

#include "../Environment/EnvShop.h"
#include "../Environment/EnvSquare.h"
#include "../Environment/EnvBoard.h"

APiece::APiece()
{
    // Enable network replication
    bReplicates = true;

    // Create and set the root component as a SceneComponent
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(Root);

    // Create the StaticMeshComponent and attach it to the root
    pieceStaticBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceBodyMesh"));

    pieceStaticBodyMesh->SetupAttachment(RootComponent);
    pieceStaticBodyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    pieceStaticBodyMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);


    // Create the BoxComponent and attach it to the root
    pieceCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    pieceCollision->SetupAttachment(RootComponent);
    pieceCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    pieceCollision->SetCollisionObjectType(ECC_WorldDynamic);
    pieceCollision->SetCollisionResponseToAllChannels(ECR_Overlap);

    PrimaryActorTick.bCanEverTick = true;
}

void APiece::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //Replicate current health.
    DOREPLIFETIME(APiece, pieceStatus);
    DOREPLIFETIME(APiece, pieceDirection);
    DOREPLIFETIME(APiece, pieceColor);

    DOREPLIFETIME(APiece, movePoint);
    DOREPLIFETIME(APiece, requireResting);
    DOREPLIFETIME(APiece, curRestingCount);
    
    DOREPLIFETIME(APiece, bHasMoved);
    DOREPLIFETIME(APiece, moveMode);

    DOREPLIFETIME(APiece, lastMoveDirection);
    DOREPLIFETIME(APiece, movementTimeline);
    DOREPLIFETIME(APiece, startLocation);
    DOREPLIFETIME(APiece, endLocation);
    DOREPLIFETIME(APiece, targetSquare);

    DOREPLIFETIME(APiece, isKilledAnyActorThisTurn);
    DOREPLIFETIME(APiece, curSquare);
    DOREPLIFETIME(APiece, isDied);
    DOREPLIFETIME(APiece, isResting);
    DOREPLIFETIME(APiece, isMoving);

    DOREPLIFETIME(APiece, isKillEffectActive);
    DOREPLIFETIME(APiece, isLaunched);

    DOREPLIFETIME(APiece, isCollided);
    DOREPLIFETIME(APiece, collisionLocation);
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
}

void APiece::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    movementTimeline.TickTimeline(DeltaTime);
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
        if (Sender->getPlayerColor() == pieceColor)
        {
            return !isMoving && !isResting;
        }
        else
        {
            return !isMoving;
        }
    }
	
}

void APiece::BeInteracted(APlayerCharacter* Sender)
{
    return;
}

// if the rest interaction is client, maybe delagate this logic into a server func
void APiece::inShopInteractedEffect_Implementation(APlayerCharacter* Sender)
{
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            AEnvShop* gameShop = GameState->getShop();

            if (gameShop)
            {
                gameShop->sellProduct(Sender, this);

                setPieceStatus(EPieceStatus::EInBench);
            }
        }
    }
}

void APiece::inBenchInteractedEffect_Implementation(APlayerCharacter* Sender) // client
{
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            AEnvBoard* gameBoard = GameState->getGameBoard();

            if (gameBoard)
            {
                gameBoard->setSpecificColor(pieceColor); // client
            }

            inBenchSpecialEffect();
        }
    }
}

void APiece::inBenchSpecialEffect_Implementation()
{
    return;
}


void APiece::inBoardInteractedEffect_Implementation(APlayerCharacter* Sender) //client
{
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            AEnvBoard* gameBoard = GameState->getGameBoard();
        
            if (gameBoard)
            {
                gameBoard->setPossibleMoves(this); // non rpc
            }
        }
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
void APiece::initializeDirection_Implementation(AEnvSquare* squareDestination)
{
    AEnvBoard* gameBoard = nullptr;
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            gameBoard = GameState->getGameBoard();

            pieceDirection = gameBoard->calculateInitDirection(squareDestination->getSquareLocation());
            /*
            switch (pieceDirection)
            {
            case EPieceDirection::EUp:
                SetActorRotation(FRotator(0.0f, 0.0f, -90.0f));
                break;
            case EPieceDirection::EDown:
                SetActorRotation(FRotator(0.0f, 0.0f, 90.0f));
                break;
            case EPieceDirection::ELeft:
                SetActorRotation(FRotator(0.0f, 0.0f, 180.0f));
                break;
            case EPieceDirection::ERight:
                SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
                break;
            default:
                SetActorRotation(FRotator(0.0f, 0.0f, 45.0f));
            }*/
        }
    }
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
        return FVector2D(-1, 0);
    case EPieceDirection::EDown:
        return FVector2D(1, 0);
    case EPieceDirection::ELeft:
        return FVector2D(0, -1);
    case EPieceDirection::ERight:
        return FVector2D(0, 1);
    case EPieceDirection::EUpLeft:
        return FVector2D(-1, -1);
    case EPieceDirection::EUpRight:
        return FVector2D(-1, 1);
    case EPieceDirection::EDownLeft:
        return FVector2D(1, -1);
    case EPieceDirection::EDownRight:
        return FVector2D(1, 1);
    default:
        return FVector2D(0, 0); // Should ideally never be hit
    }
}

EPieceDirection APiece::directionVectorToEnum(FVector2D DirectionVector) const
{
    // Mapping direction vectors to the corresponding enum values for all eight directions
    if (DirectionVector == FVector2D(-1, 0)) return EPieceDirection::EUp;
    if (DirectionVector == FVector2D(1, 0)) return EPieceDirection::EDown;
    if (DirectionVector == FVector2D(0, -1)) return EPieceDirection::ELeft;
    if (DirectionVector == FVector2D(0, 1)) return EPieceDirection::ERight;
    if (DirectionVector == FVector2D(-1, -1)) return EPieceDirection::EUpLeft;
    if (DirectionVector == FVector2D(-1, 1)) return EPieceDirection::EUpRight;
    if (DirectionVector == FVector2D(1, -1)) return EPieceDirection::EDownLeft;
    if (DirectionVector == FVector2D(1, 1)) return EPieceDirection::EDownRight;
    return EPieceDirection::ENone; // Return a default case if no direction matches
}

TArray<FVector2D> APiece::getDiagonalDirections() const
{
    TArray<FVector2D> DiagonalDirections;
    // Add diagonal vectors depending on the piece's direction
    switch (pieceDirection)
    {
    case EPieceDirection::EUp:
        DiagonalDirections.Add(FVector2D(-1, -1));  // Top-left
        DiagonalDirections.Add(FVector2D(-1, 1));   // Top-right
        break;
    case EPieceDirection::EDown:
        DiagonalDirections.Add(FVector2D(1, -1)); // Bottom-left
        DiagonalDirections.Add(FVector2D(1, 1));  // Bottom-right
        break;
    case EPieceDirection::ELeft: // Not typically used but included for completeness
        DiagonalDirections.Add(FVector2D(-1, -1)); // Left-Top
        DiagonalDirections.Add(FVector2D(1, -1));  // Left-Down
        break;
    case EPieceDirection::ERight: // Not typically used but included for completeness
        DiagonalDirections.Add(FVector2D(-1, 1));  // Right-bottom
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

    AEnvBoard* gameBoard = nullptr;
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            gameBoard = GameState->getGameBoard();
        }
    }

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

    AEnvBoard* gameBoard = nullptr;
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            gameBoard = GameState->getGameBoard();
        }
    }
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

    AEnvBoard* gameBoard = nullptr;
    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            gameBoard = GameState->getGameBoard();
        }
    }

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

EPieceDirection APiece::getOppositeDirection(EPieceDirection Direction)
{
    switch (Direction)
    {
    case EPieceDirection::EUp:
        return EPieceDirection::EDown;
    case EPieceDirection::EUpLeft:
        return EPieceDirection::EDownRight;
    case EPieceDirection::ELeft:
        return EPieceDirection::ERight;
    case EPieceDirection::EDownLeft:
        return EPieceDirection::EUpRight;
    case EPieceDirection::EDown:
        return EPieceDirection::EUp;
    case EPieceDirection::EDownRight:
        return EPieceDirection::EUpLeft;
    case EPieceDirection::ERight:
        return EPieceDirection::ELeft;
    case EPieceDirection::EUpRight:
        return EPieceDirection::EDownLeft;
    default:
        return EPieceDirection::ENone;
    }

    return EPieceDirection::ENone;
}

void APiece::die_Implementation(APiece* killer) // server
{
    if (!isDied)
    {
        isDied = true;

        if (curSquare)
        {
            curSquare->occupiedPieceLeaved();
        }
        
        dieEffect(killer);
    }
}

void APiece::dieEffect_Implementation(APiece* killer) // netmulticast
{
    FVector Direction = FVector::ZeroVector;
    FVector MyLocation = GetActorLocation();
    if (killer)
    {
        FVector KillerLocation = killer->GetActorLocation();
        Direction = (MyLocation - KillerLocation).GetSafeNormal();
    }
    else
    {
        Direction = (MyLocation - collisionLocation).GetSafeNormal();
    }

    spawnFractureMesh(Direction); // client

    Destroy();
}

void APiece::spawnFractureMesh_Implementation(FVector aDirection) // client
{
    if (pieceFractureMeshClass)
    {
        UWorld* serverWorld = GetWorld();
        if (serverWorld)
        {
            FVector RandomOffset = FMath::RandPointInBox(FBox(FVector(-randomSpawnFractureDist, -randomSpawnFractureDist, -randomSpawnFractureDist), FVector(randomSpawnFractureDist, randomSpawnFractureDist, randomSpawnFractureDist)));

            FVector SpawnLocation = GetActorLocation() + RandomOffset;

            APieceFractureMesh* fractureMesh = serverWorld->SpawnActor<APieceFractureMesh>(pieceFractureMeshClass, SpawnLocation, GetActorRotation());
            if (fractureMesh)
            {
                fractureMesh->setMaterial(selectedMaterial);
                fractureMesh->applyForce(aDirection * collideImpulseStrength);
            }
        }
    }
}

AEnvSquare* APiece::getOccupiedSquare()
{
    return curSquare;
}
void APiece::setOccupiedSquare(AEnvSquare* aSquare)
{
    curSquare = aSquare;
}

int APiece::getPiecePriority()
{
    return collisionPriority;
}

bool APiece::getIsMoving()
{
    return isMoving;
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
            if (OtherActor->IsA(APiece::StaticClass()))
            {
                if (OtherActor != this)
                {
                    APiece* collidedPiece = Cast<APiece>(OtherActor);
                    collidedWithOtherPiece(collidedPiece);
                }
            }
            else if (OtherActor->IsA(APlayerCharacter::StaticClass()))
            {
                APlayerCharacter* playerCollided = Cast<APlayerCharacter>(OtherActor);
                isKilledAnyActorThisTurn = true;
                playerCollided->beCollidedByPiece(this);
            }
        }
    }
}

void APiece::collidedWithOtherPiece(APiece* collidedPiece)
{
    if (collidedPiece)
    {
        collidedPiece->setIsCollidedBy(this); // disable the other piece collision calculation
        setIsCollidedBy(collidedPiece);

        // DrawDebugLine(GetWorld(), GetActorLocation(), collidedPiece->GetActorLocation(), FColor::Blue, false, 10, 0, 10);

        if (collidedPiece->getIsMoving())
        {
            if (collisionPriority > collidedPiece->getPiecePriority())
            {
                kill(collidedPiece);
            }
            else if (collisionPriority < collidedPiece->getPiecePriority())
            {
                collidedPiece->kill(this);
            }
            else
            {
                kill(collidedPiece);
                die(collidedPiece);
            }
        }
        else
        {
            kill(collidedPiece);
        }
    }
}

void APiece::setIsCollidedBy(APiece* collidedPiece)
{
    if (collidedPiece)
    {
        isCollided = true;
        collisionLocation = collidedPiece->GetActorLocation();
    }
    else
    {
        isCollided = true;
        collisionLocation = GetActorLocation();
    }
}

void APiece::kill(APiece* pieceToKill)
{
    if (pieceToKill)
    {
        pieceToKill->die(this);
        isKilledAnyActorThisTurn = true;
    }
}

/* piece moving*/
EPieceStatus APiece::getPieceStatus()
{
    return pieceStatus;
}
void APiece::setPieceStatus_Implementation(EPieceStatus newStatus)
{
    pieceStatus = newStatus;
}
void APiece::setPieceStatusInBoard_Implementation()
{
    pieceStatus = EPieceStatus::EInBoard;
}

void APiece::setPieceColor_Implementation(FColor aColor)
{
    pieceColor = aColor;

    if (colorToMaterial.Contains(aColor))
    {
        selectedMaterial = colorToMaterial[aColor];

        if (selectedMaterial && pieceStaticBodyMesh)
        {
            int32 MaterialCount = pieceStaticBodyMesh->GetNumMaterials(); // Get the number of materials on the mesh
            for (int32 Index = 0; Index < MaterialCount; ++Index)
            {
                pieceStaticBodyMesh->SetMaterial(Index, selectedMaterial); // Set the material for each index
            }
        }

        pieceStaticBodyMesh->SetVisibility(true); // Ensure the piece is visible after setting the materials
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

void APiece::updateStatusByTurn_Implementation()
{
    updateRestStatus();
}
void APiece::updateRestStatus_Implementation()
{
    if (isResting)
    {
        curRestingCount++;
        if (requiredRestingTurn % curRestingCount == 0)
        {
            isResting = false;
            curRestingCount = 0;
        }
    }
}

/* setActor location in a time constraint*/
void APiece::bePlaced_Implementation(AEnvSquare* squareDestination)
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

        if (specialPossibleMove.Contains(squareDestination->getSquareLocation()))
        {
            bePlacedSpecialSquareEffect(squareDestination);
        }
    }

    if (UWorld* World = GetWorld())
    {
        ARLGameState* GameState = Cast<ARLGameState>(World->GetGameState());
        if (GameState)
        {
            AEnvBoard* gameBoard = GameState->getGameBoard();
        

            if (gameBoard)
            {
                gameBoard->resetBoard();
            }
        }
    }
}

void APiece::setLocationMulti_Implementation(FVector aLocation)
{
    SetActorLocation(aLocation);
}

void APiece::bePlacedInShopEffect_Implementation(AEnvSquare* squareDestination)
{
    // setPieceStatus(EPieceStatus::EInBench);

    if (IsValid(curSquare))
    {
        curSquare->occupiedPieceLeaved();
    }

    // teleport to squareLocation
    curSquare = squareDestination;
    initializeDirection(curSquare); // initialize the direction
    
    curSquare->beOccupied(this);
    setLocationMulti(curSquare->getPlacementLocation());
}

void APiece::bePlacedInBenchEffect_Implementation(AEnvSquare* squareDestination)
{
    // enable swap position
    if (squareDestination->getIsOccupied())
    {
        swapLocation(squareDestination);
    }
    else
    {
        if (IsValid(curSquare))
        {
            curSquare->occupiedPieceLeaved();
        }

        // teleport to squareLocation
        curSquare = squareDestination;
        curSquare->beOccupied(this);
        setLocationMulti(curSquare->getPlacementLocation());
    }
}

void APiece::swapLocation_Implementation(AEnvSquare* squareDestination)
{
    if (squareDestination && curSquare)
    {
        APiece* pieceToSwap = squareDestination->getOccupiedPiece();
        if (pieceToSwap)
        {
            if (IsValid(curSquare))
            {
                curSquare->occupiedPieceLeaved();
            } 
            // avoid collision
            setLocationMulti(curSquare->getPlacementLocation() - FVector(0.0f, 0.0f, -3000.0f));
        
            // let the piece occupy this position
            pieceToSwap->setLocationMulti(curSquare->getPlacementLocation());
            pieceToSwap->setOccupiedSquare(curSquare);
            curSquare->beOccupied(pieceToSwap);

            // teleport to squareLocation
            curSquare->beOccupied(this);
            setLocationMulti(squareDestination->getPlacementLocation());

            curSquare = squareDestination;
        }
    }
}

// all pieces are inboard when setup time is finished
void APiece::bePlacedInBoardEffect_Implementation(AEnvSquare* squareDestination)
{
    if (!bHasMoved)
    {
        firstInBoardMovedEffect(squareDestination);
    }
    
    if (!isMoving) // only move if it is not moving
    {
        startMoving(squareDestination);
    }
}

void APiece::firstInBoardMovedEffect_Implementation(AEnvSquare* squareDestination)
{
    bHasMoved = true;
}

void APiece::bePlacedSpecialSquareEffect_Implementation(AEnvSquare* squareDestination)
{
    return;
}

void APiece::startMoving_Implementation(AEnvSquare* squareDestination)
{
    if (IsValid(curSquare))
    {
        curSquare->occupiedPieceLeaved();
    }

    lastMoveDirection = calculateMovingDirection(squareDestination);

    isMoving = true;
    isKilledAnyActorThisTurn = false;
    startLocation = GetActorLocation();
    endLocation = squareDestination->getPlacementLocation();
    targetSquare = squareDestination;

    moveBasedOnMove(squareDestination);
}

EPieceDirection APiece::calculateMovingDirection(AEnvSquare* squareDestination)
{
    if (!curSquare || !squareDestination)
    {
        return EPieceDirection::ENone;  // Check for valid pointers
    }

    int RowDiff = squareDestination->getSquareLocation().X - curSquare->getSquareLocation().X;
    int ColDiff = squareDestination->getSquareLocation().Y - curSquare->getSquareLocation().Y;

    // Determine the direction based on the row and column differences
    if (RowDiff == 0 && ColDiff > 0)
    {
        return EPieceDirection::ERight;
    }
    else if (RowDiff == 0 && ColDiff < 0)
    {
        return EPieceDirection::ELeft;
    }
    else if (RowDiff > 0 && ColDiff == 0)
    {
        return EPieceDirection::EDown;
    }
    else if (RowDiff < 0 && ColDiff == 0)
    {
        return EPieceDirection::EUp;
    }
    else if (RowDiff < 0 && ColDiff > 0)
    {
        return EPieceDirection::EUpRight;
    }
    else if (RowDiff < 0 && ColDiff < 0)
    {
        return EPieceDirection::EUpLeft;
    }
    else if (RowDiff > 0 && ColDiff > 0)
    {
        return EPieceDirection::EDownRight;
    }
    else if (RowDiff > 0 && ColDiff < 0)
    {
        return EPieceDirection::EDownLeft;
    }

    return EPieceDirection::ENone;  // Default return if no direction is determined
}

void APiece::endMoving_Implementation()
{
    setLocationMulti(endLocation);
    isMoving = false;

    curSquare = targetSquare;
    // teleport to squareLocation
    curSquare->beOccupied(this);

    if (requireResting)
    {
        isResting = true;
    }

    if (isKillEffectActive && isKilledAnyActorThisTurn)
    {
        killEffect();
    }

    if (isLaunched)
    {
        launchEndEffect();
    }
}

void APiece::killEffect_Implementation()
{
    return;
}

void APiece::moveBasedOnMove_Implementation(AEnvSquare* squareDestination)
{
    switch (moveMode)
    {
    case EPieceMoveMode::EGround:
    {
        initiateGroundMovement(squareDestination);
        break;
    }

    case EPieceMoveMode::EParabolicJump:
    {
        initiateParabolicJump(squareDestination);
        break;
    }

    case EPieceMoveMode::EStaticJump:
    {
        initiateKnightJump(squareDestination);
        break;
    }
    case EPieceMoveMode::ETeleport:
    {
        initializeTeleportation(squareDestination);
        break;
    }
    default:
        break;
    }
}

void APiece::initiateGroundMovement_Implementation(AEnvSquare* squareDestination)
{
    if (!movementCurve) return; // Ensure the curve asset is set			

    FOnTimelineFloat ProgressFunction;
    ProgressFunction.BindUFunction(this, FName("handleGroundMovementProgress"));
    movementTimeline.AddInterpFloat(movementCurve, ProgressFunction);
    
    
    FOnTimelineEvent FinishFunction;
    FinishFunction.BindUFunction(this, FName("endMoving"));
    movementTimeline.SetTimelineFinishedFunc(FinishFunction);
    
    movementTimeline.SetPlayRate(1.0f / movementDuration);
    movementTimeline.SetLooping(false);

    movementTimeline.PlayFromStart();			
}

void APiece::handleGroundMovementProgress_Implementation(float value)
{
    // Linearly interpolate position based on the timeline's progress			
    FVector newLocation = FMath::Lerp(startLocation, endLocation, value);
    setLocationMulti(newLocation);
}

void APiece::initiateParabolicJump_Implementation(AEnvSquare* SquareDestination)
{
    if (!movementCurve) return; // Ensure the curve asset is set			

    FOnTimelineFloat ProgressFunction;
    ProgressFunction.BindUFunction(this, FName("handleParabolicJumpProgress"));
    movementTimeline.AddInterpFloat(movementCurve, ProgressFunction);

    FOnTimelineEvent FinishFunction;
    FinishFunction.BindUFunction(this, FName("endMoving"));
    movementTimeline.SetTimelineFinishedFunc(FinishFunction);

    movementTimeline.SetLooping(false);
    movementTimeline.PlayFromStart();
}

void APiece::handleParabolicJumpProgress_Implementation(float value)
{
    FVector NewLocation = FMath::Lerp(startLocation, endLocation, value);
    float Parabola = -4 * jumpApexHeight * value * (value - 1);  // Parabolic formula			
    NewLocation.Z += Parabola;
    setLocationMulti(NewLocation);
}

void APiece::initiateKnightJump_Implementation(AEnvSquare* SquareDestination)
{
    FOnTimelineFloat ProgressFunction;
    ProgressFunction.BindUFunction(this, FName("handleKnightJumpProgress"));
    movementTimeline.AddInterpFloat(movementCurve, ProgressFunction);

    FOnTimelineEvent FinishFunction;
    FinishFunction.BindUFunction(this, FName("endMoving"));
    movementTimeline.SetTimelineFinishedFunc(FinishFunction);
    movementTimeline.SetLooping(false);
    movementTimeline.PlayFromStart();
}

void APiece::handleKnightJumpProgress_Implementation(float value)
{   
    // Interpolate the horizontal position
    FVector HorizontalMovement = FMath::Lerp(FVector(startLocation.X, startLocation.Y, 0), FVector(endLocation.X, endLocation.Y, 0), value);

    // Calculate the vertical movement using a half sine wave that starts and ends at 0 height difference
    float VerticalMovement = jumpApexHeight * FMath::Sin(FMath::DegreesToRadians(180.0f * value));

    // Add the original starting Z position to the vertical movement
    FVector FinalPosition = HorizontalMovement + FVector(0, 0, startLocation.Z + VerticalMovement);

    setLocationMulti(FinalPosition);
}

void APiece::initializeTeleportation_Implementation(AEnvSquare* SquareDestination)
{
    // Ensure we have a valid world context before setting a timer			
    if (GetWorld())
    {
        // Clears any existing timer to avoid multiple teleportations			
        GetWorld()->GetTimerManager().ClearTimer(TeleportTimerHandle);

        // Set a timer to teleport the actor			
        FTimerDelegate TimerDel;
        TimerDel.BindUFunction(this, FName("endMoving"), SquareDestination);
        GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, TimerDel, movementDuration, false);
    }
}

void APiece::beLaunchedTo_Implementation(AEnvSquare* SquareDestination)
{
    isLaunched = true;
    moveMode = EPieceMoveMode::EParabolicJump;

    startMoving(SquareDestination);
}

void APiece::launchEndEffect_Implementation()
{
    die(this);
}

void APiece::beExploted_Implementation()
{
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

        // Directions rook can move: up, down, left, right
        TArray<EPieceDirection> Directions = {
            EPieceDirection::EUp, EPieceDirection::EDown,
            EPieceDirection::ELeft, EPieceDirection::ERight,
            EPieceDirection::EUpLeft, EPieceDirection::EDownLeft,
            EPieceDirection::EUpRight, EPieceDirection::EDownRight,
        };

        // Iterate over each direction and calculate possible moves
        for (EPieceDirection Direction : Directions)
        {
            TArray<FVector2D> allMovesLocation = getLineMoveWithFirstObstacle(CurrentLocation, Direction, movePoint);
            for (FVector2D moveLocation : allMovesLocation)
            {
                AEnvSquare* checkSquare = gameBoard->getSquareAtLocation(moveLocation);

                if (checkSquare)
                {
                    if (checkSquare->getIsOccupied())
                    {
                        checkSquare->getOccupiedPiece()->die(this);
                    }
                }
            }
        }
    }

    die(this);
}


/* additional decoration*/
APiecePreviewMesh* APiece::getSpawnedPreviewMesh(FVector locationToSpawn)
{
    if (piecePreviewMeshClass)
    {
        UWorld* serverWorld = GetWorld();
        if (serverWorld)
        {
            APiecePreviewMesh* previewMesh = serverWorld->SpawnActor<APiecePreviewMesh>(piecePreviewMeshClass, locationToSpawn, GetActorRotation());
            // previewMesh->setMaterialColor(selectedMaterial);

            return previewMesh;
        }
    }
    return nullptr;
}

APieceConfirmedMesh* APiece::getSpawnedConfirmedMesh(FVector locationToSpawn)
{
    if (pieceConfirmedMeshClass)
    {
        UWorld* serverWorld = GetWorld();
        if (serverWorld)
        {
            APieceConfirmedMesh* confirmedMesh = serverWorld->SpawnActor<APieceConfirmedMesh>(pieceConfirmedMeshClass, locationToSpawn, GetActorRotation());

            return confirmedMesh;
        }
    }
    return nullptr;
}
