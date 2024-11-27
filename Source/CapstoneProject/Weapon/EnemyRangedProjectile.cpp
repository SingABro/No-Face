// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/EnemyRangedProjectile.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"

AEnemyRangedProjectile::AEnemyRangedProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->OnComponentBeginOverlap.AddDynamic(this, &AEnemyRangedProjectile::OnOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Box);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	Direction = FVector::ZeroVector;
	LifeTime = 2.5f;
}

void AEnemyRangedProjectile::BeginPlay()
{
	Super::BeginPlay();


}

void AEnemyRangedProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (Direction * 1500.f * DeltaTime);
	SetActorLocation(NewLocation);

	LifeTime -= DeltaTime;
	if (LifeTime <= 0.f)
	{
		Destroy();
	}
}

void AEnemyRangedProjectile::Init(const FVector& InDirection)
{
	Direction = InDirection;
}

void AEnemyRangedProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		FDamageEvent DamageEvent;
		if (!GetOwner()) return;
		OtherActor->TakeDamage(200.f, DamageEvent, GetWorld()->GetFirstPlayerController(), GetOwner());
		Destroy();
	}
}
