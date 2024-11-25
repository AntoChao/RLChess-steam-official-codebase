// Copyright Epic Games, Inc. All Rights Reserved.

#include "Piece.h"
#include "../Environment/EnvShop.h"
#include "../Player/PlayerCharacter.h"
#include "../Environment/EnvSquare.h"
#include "../Environment/EnvBoard.h"

APiece::APiece()
{
    pieceBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
    SetRootComponent(pieceBody);

    pieceCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    pieceCollision->SetupAttachment(pieceBody);

    // Configure Collision Settings
    pieceCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    pieceCollision->SetCollisionObjectType(ECC_WorldDynamic);
    pieceCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
    pieceCollision->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
}

void APiece::BeginPlay()
{
    Super::BeginPlay();

    // Bind Overlap Event
    if (pieceCollision)
    {
        pieceCollision->OnComponentBeginOverlap.AddDynamic(this, &APiece::OnOverlapBegin);
    }
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
	return Sender->getPlayerColor() == pieceColor;
}

void APiece::BeInteracted(APlayerCharacter* Sender)
{
    switch (pieceStatus)
    {
        case EPieceStatus::EInShop:
        {
            /*if (Sender->isAbleToBenchPiece())
            {
                Sender->benchAPiece(this);

                AEnvShop* gameShop = AEnvShop::get();
                gameShop->sellProduct(Sender, this);
            }*/
            break;
        }
        case EPieceStatus::EInBench:
        {
            AEnvBoard* gameBoard = AEnvBoard::get();
            gameBoard->setSpecificColor(pieceColor);
            break;
        }
        case EPieceStatus::EInBoard:
        {
            TArray<FVector2D> allPossibles = calculatePossibleMove();

            AEnvBoard* gameBoard = AEnvBoard::get();
            gameBoard->setPossibleMoves(allPossibles);
            break;
        }
        default:
            break;
    }
}

void APiece::BeUnInteracted(APlayerCharacter* Sender)
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

int APiece::getLevel()
{
    return pieceLevel;
}

/* setActor location in a time constraint*/
void APiece::bePlaced(AEnvSquare* squareDestination)
{
    if (IsValid(curSquare))
    {
        curSquare->occupiedPieceLeaved();
    }

    curSquare = squareDestination;

    // move to squareLocation
    SetActorLocation(curSquare->GetActorLocation());
}