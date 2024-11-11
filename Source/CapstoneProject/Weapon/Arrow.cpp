// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Arrow.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "Stat/CharacterDataStat.h"

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetCollisionProfileName(TEXT("Arrow"));
	Box->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Box);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/MRPGT/StaticMeshes/Arrow/SM_Arrow_A.SM_Arrow_A'"));
	if (MeshRef.Object)
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}

	Direction = FVector::ZeroVector;
	LifeTime = 5.f;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();

}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (Direction * Stat->BowSpeed * DeltaTime);
	SetActorLocation(NewLocation);

	LifeTime -= DeltaTime;
	if (LifeTime <= 0.f)
	{
		Destroy();
	}
}

void AArrow::Init(const FVector& InDirection)
{
	Direction = InDirection;
}

void AArrow::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && !OtherActor->ActorHasTag(TEXT("Player")))
	{
		FDamageEvent DamageEvent;
		AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
		if (Enemy)
		{
			if (GetOwner() == nullptr) return;
			Enemy->TakeDamage(Stat->BowDamage, DamageEvent, GetWorld()->GetFirstPlayerController(), GetOwner(), TEXT("Default"));
		}
		Destroy();
	}
}