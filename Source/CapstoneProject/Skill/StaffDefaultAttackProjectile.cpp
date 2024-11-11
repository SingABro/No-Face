// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/StaffDefaultAttackProjectile.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/DamageEvents.h"
#include "Stat/CharacterDataStat.h"

AStaffDefaultAttackProjectile::AStaffDefaultAttackProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->OnComponentBeginOverlap.AddDynamic(this, &AStaffDefaultAttackProjectile::OnBeginOverlap);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(Box);
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComponent->SetStaticMesh(Mesh);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleComponent->SetupAttachment(Box);
	ParticleComponent->SetTemplate(Particle);
	ParticleComponent->bAutoActivate = true;

	MoveDirection = FVector::ZeroVector;
	
	LifeTime = 5.f;
}

void AStaffDefaultAttackProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AStaffDefaultAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (MoveDirection * Stat->StaffSpeed * DeltaTime);
	SetActorLocation(NewLocation);

	LifeTime -= DeltaTime;
	if (LifeTime <= 0.f)
	{
		Destroy();
	}

}

void AStaffDefaultAttackProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor)
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
		if (Enemy)
		{
			if (GetOwner() == nullptr) return;
			FDamageEvent DamageEvent;
			Enemy->TakeDamage(Stat->StaffDamage, DamageEvent, GetWorld()->GetFirstPlayerController(), GetOwner(), TEXT("Staff_Default"));
		}
	}
}

void AStaffDefaultAttackProjectile::Init(const FVector& InMoveDirection)
{
	MoveDirection = InMoveDirection;
}
