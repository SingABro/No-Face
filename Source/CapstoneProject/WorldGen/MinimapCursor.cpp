// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGen/MinimapCursor.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AMinimapCursor::AMinimapCursor()
{
}

void AMinimapCursor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Player)
	{
		NewLocation = UKismetMathLibrary::TransformLocation(UKismetMathLibrary::MakeTransform(FVector(10000.f, 10000.f, 8010.f),
			FRotator::ZeroRotator, FVector(0.2f, 0.2f, 0.2f)), 
			Player->GetActorLocation() - (FVector::RightVector * PlusYWarpIndex * 5000.f) - (-FVector::RightVector * MinusYWarpIndex * 5000.f)
			- (FVector::ForwardVector * PlusXWarpIndex * 5000.f) - (-FVector::ForwardVector * MinusXWarpIndex * 5000.f));
		float RollRot;
		float PitchRot;
		float YawRot;
		UKismetMathLibrary::BreakRotator(Player->GetActorRotation(), RollRot, PitchRot, YawRot);
		FRotator NewRotator = UKismetMathLibrary::MakeRotator(90.f, 0.f, YawRot - 90.f);

		//UE_LOG(LogTemp, Display, TEXT("Cursor : %f, %f, %f"), NewLocation.X, NewLocation.Y, NewLocation.Z);


		SetActorLocationAndRotation(NewLocation, NewRotator);
	}
}

void AMinimapCursor::WarpEvent(const FVector& InWarpLocation)
{
	if (InWarpLocation == FVector::RightVector)
	{
		PlusYWarpIndex++;
	}
	else if (InWarpLocation == -FVector::RightVector)
	{
		MinusYWarpIndex++;
	}
	else if(InWarpLocation == FVector::ForwardVector)
	{
		PlusXWarpIndex++;
	}
	else if(InWarpLocation == -FVector::ForwardVector)
	{
		MinusXWarpIndex++;
	}
	UE_LOG(LogTemp, Display, TEXT("X : %d / Y : %d / -X : %d / -Y : %d"), PlusXWarpIndex, PlusYWarpIndex, MinusXWarpIndex, MinusYWarpIndex);
}

void AMinimapCursor::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACharacterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass()));
	Player->OnWarpNextMap.AddUObject(this, &AMinimapCursor::WarpEvent);
}
