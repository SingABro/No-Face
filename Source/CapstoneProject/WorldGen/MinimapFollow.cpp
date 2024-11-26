// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGen/MinimapFollow.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AMinimapFollow::AMinimapFollow()
{
}

void AMinimapFollow::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Player)
	{
		NewLocation = UKismetMathLibrary::TransformLocation(UKismetMathLibrary::MakeTransform(FVector(10000.f, 10000.f, 8010.f),
			FRotator::ZeroRotator, FVector(0.2f, 0.2f, 0.2f)),
			Player->GetActorLocation() - (FVector::RightVector * PlusYWarpIndex * 5500.f) - (-FVector::RightVector * MinusYWarpIndex * 5500.f)
			- (FVector::ForwardVector * PlusXWarpIndex * 5500.f) - (-FVector::ForwardVector * MinusXWarpIndex * 5500.f));

		//UE_LOG(LogTemp, Display, TEXT("Follow : %f, %f, %f"), NewLocation.X, NewLocation.Y, NewLocation.Z);

		SetActorLocation(NewLocation);
	}
}

void AMinimapFollow::WarpEvent(const FVector& InWarpLocation)
{
	if (InWarpLocation == FVector::RightVector)
	{
		PlusYWarpIndex++;
	}
	else if (InWarpLocation == -FVector::RightVector)
	{
		MinusYWarpIndex++;
	}
	else if (InWarpLocation == FVector::ForwardVector)
	{
		PlusXWarpIndex++;
	}
	else if (InWarpLocation == -FVector::ForwardVector)
	{
		MinusXWarpIndex++;
	}
	UE_LOG(LogTemp, Display, TEXT("X : %d / Y : %d / -X : %d / -Y : %d"), PlusXWarpIndex, PlusYWarpIndex, MinusXWarpIndex, MinusYWarpIndex);
}

void AMinimapFollow::BeginPlay()
{
	Super::BeginPlay();

	if (Player)
	{
		Player->OnWarpNextMap.AddUObject(this, &AMinimapFollow::WarpEvent);
	}
	else
	{
		FTimerHandle Temp;
		GetWorld()->GetTimerManager().SetTimer(Temp, [&]()
			{
				Init();
			}, 0.5f, true);
	}
}

void AMinimapFollow::Init()
{
	Player = Cast<ACharacterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass()));
}
