// Copyright Epic Games, Inc. All Rights Reserved.

#include "Piece.h"

#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Curves/CurveFloat.h"
#include "Math/UnrealMathUtility.h"

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
    pieceStaticBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destructable Body"));

    pieceStaticBody->SetupAttachment(RootComponent);
    pieceStaticBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    pieceStaticBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);


    // Create the BoxComponent and attach it to the root
    pieceCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    pieceCollision->SetupAttachment(RootComponent);
    pieceCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    pieceCollision->SetCollisionObjectType(ECC_WorldDynamic);
    pieceCollision->SetCollisionResponseToAllChannels(ECR_Overlap);

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
        return Sender->getPlayerColor() == pieceColor && !isMoving && !isResting;
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
        
        setPieceStatus(EPieceStatus::EInBench);
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

void APiece::die(APiece* killer)
{
    dieEffect(killer);
}

void APiece::dieEffect(APiece* killer)
{
    DrawDebugPoint(GetWorld(), GetActorLocation(), 300.0f, FColor::Red, false, 5.0f);

    if (killer)
    {
        FVector KillerLocation = killer->GetActorLocation();
        FVector MyLocation = GetActorLocation();

        // Calculate the direction vector from the killer to this piece
        FVector Direction = (MyLocation - KillerLocation).GetSafeNormal();

        

        // Apply the impulse to the piece's body
        pieceStaticBody->AddImpulse(Direction * collideImpulseStrength, NAME_None, true);
    }

    // Additional logic to handle the piece's death, such as removing it from the game board

    if (isLaunched)
    {
        beExploted();
    }
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

                DrawDebugLine(GetWorld(), GetActorLocation(), OtherActor->GetActorLocation() + FVector(0.0f, 0.0f, 500.0f), FColor::Blue, false, 1, 0, 1);

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
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("piece be collided with other piece"));
    
    if (collidedPiece)
    {
        if (collidedPiece->getIsMoving())
        {
            if (collisionPriority > collidedPiece->getPiecePriority())
            {
                kill(collidedPiece);
            }
            else if (collisionPriority > collidedPiece->getPiecePriority())
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

void APiece::kill(APiece* pieceToKill)
{
    pieceToKill->die(this);
    isKilledAnyActorThisTurn = true;
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

        if (SelectedMaterial && pieceStaticBody)
        {
            int32 MaterialCount = pieceStaticBody->GetNumMaterials(); // Get the number of materials on the mesh
            for (int32 Index = 0; Index < MaterialCount; ++Index)
            {
                pieceStaticBody->SetMaterial(Index, SelectedMaterial); // Set the material for each index
            }
        }

        pieceStaticBody->SetVisibility(true); // Ensure the piece is visible after setting the materials
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

void APiece::updateStatus()
{
    updateRestStatus();
}
void APiece::updateRestStatus()
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
void APiece::bePlaced(AEnvSquare* squareDestination)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("PIECE BE PLACED"));

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
    // setPieceStatus(EPieceStatus::EInBench);

    if (IsValid(curSquare))
    {
        curSquare->occupiedPieceLeaved();
    }

    // teleport to squareLocation
    curSquare = squareDestination;
    initializeDirection(curSquare); // initialize the direction

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
    if (specialPossibleMove.Contains(squareDestination->getSquareLocation()))
    {
        bePlacedSpecialSquareEffect(squareDestination);
    }
    
    if (!isMoving) // only move if it is not moving
    {
        startMoving(squareDestination);
    }
}

void APiece::firstInBoardMovedEffect(AEnvSquare* squareDestination)
{
    bHasMoved = true;
}

void APiece::bePlacedSpecialSquareEffect(AEnvSquare* squareDestination)
{
    return;
}

void APiece::startMoving(AEnvSquare* squareDestination)
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

void APiece::endMoving()
{
    SetActorLocation(endLocation);
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

void APiece::killEffect()
{
    return;
}

void APiece::moveBasedOnMove(AEnvSquare* squareDestination)
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

void APiece::initiateGroundMovement(AEnvSquare* squareDestination)
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

void APiece::handleGroundMovementProgress(float value)
{
    // Linearly interpolate position based on the timeline's progress			
    FVector newLocation = FMath::Lerp(startLocation, endLocation, value);
    SetActorLocation(newLocation);
}

void APiece::initiateParabolicJump(AEnvSquare* SquareDestination)
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

void APiece::handleParabolicJumpProgress(float value)
{
    FVector NewLocation = FMath::Lerp(startLocation, endLocation, value);
    float Parabola = -4 * jumpApexHeight * value * (value - 1);  // Parabolic formula			
    NewLocation.Z += Parabola;
    SetActorLocation(NewLocation);
}

void APiece::initiateKnightJump(AEnvSquare* SquareDestination)
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

void APiece::handleKnightJumpProgress(float value)
{   
    // Interpolate the horizontal position
    FVector HorizontalMovement = FMath::Lerp(FVector(startLocation.X, startLocation.Y, 0), FVector(endLocation.X, endLocation.Y, 0), value);

    // Calculate the vertical movement using a half sine wave that starts and ends at 0 height difference
    float VerticalMovement = jumpApexHeight * FMath::Sin(FMath::DegreesToRadians(180.0f * value));

    // Add the original starting Z position to the vertical movement
    FVector FinalPosition = HorizontalMovement + FVector(0, 0, startLocation.Z + VerticalMovement);

    SetActorLocation(FinalPosition);
}

void APiece::initializeTeleportation(AEnvSquare* SquareDestination)
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

void APiece::beLaunchedTo(AEnvSquare* SquareDestination)
{
    isLaunched = true;
    moveMode = EPieceMoveMode::EParabolicJump;

    startMoving(SquareDestination);
}

void APiece::launchEndEffect()
{
    die(this);
}

void APiece::beExploted()
{
    AEnvBoard* gameBoard = UMapManager::get()->getGameBoard();
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
                    else if (checkSquare->getIsPlayerOnTop())
                    {
                        checkSquare->getPlayerOnTop()->startDying();
                    }
                }
            }
        }
    }
}