// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/StaffDefaultAttackProjectile.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

AStaffDefaultAttackProjectile::AStaffDefaultAttackProjectile()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->OnComponentBeginOverlap.AddDynamic(this, &AStaffDefaultAttackProjectile::OnBeginOverlap);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleComponent->SetupAttachment(Box);
	ParticleComponent->SetTemplate(Particle);
	ParticleComponent->bAutoActivate = true;

	MoveDirection = FVector::ZeroVector;
	Damage = 200.f;
	MoveSpeed = 500.f;;
	LifeTime = 5.f;
}

void AStaffDefaultAttackProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AStaffDefaultAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (MoveDirection * MoveSpeed * DeltaTime);
	SetActorLocation(NewLocation);

	LifeTime -= DeltaTime;
	if (LifeTime <= 0.f)
	{
		Destroy();
	}
}

void AStaffDefaultAttackProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AStaffDefaultAttackProjectile::Init(const FVector& InMoveDirection)
{
	MoveDirection = InMoveDirection;
}
