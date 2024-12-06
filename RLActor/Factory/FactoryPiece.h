#pragma once

#include "CoreMinimal.h"
#include "RLFactory.h"
#include "../Piece/Piece.h"
#include "FactoryPiece.generated.h"

UCLASS(Blueprintable)
class UFactoryPiece : public URLFactory
{
	GENERATED_BODY()
	
public:
	virtual AActor* createRLActor(const FString& name, FVector aLocation, FRotator aRotation) override;

	virtual AActor* createRandom(FVector aLocation, FRotator aRotation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	int totalPiecesClassNum = 0;

	// classic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> pawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> rookClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> knightClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> bishopClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> queenClass;

	// extra
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> cannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> rangerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> assassinClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> mageClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> samuraiClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> cultistClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> scoutClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> cowboyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> warrriorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> doggyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> doctorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> witchClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> guardClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> ninjaClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> catapultClass;

	// no implemented yet
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece Factory Stats")
	TSubclassOf<APiece> bountyHunterClass;

};

