// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/StaffDefaultAttackProjectile.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/DamageEvents.h"

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

	DrawDebugSphere(GetWorld(), NewLocation, 50.f, 32, FColor::Red, false);
}

void AStaffDefaultAttackProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner() && OtherActor)
	{
		FDamageEvent DamageEvent;
		if (GetOwner())
		{
			OtherActor->TakeDamage(Damage, DamageEvent, GetInstigatorController(), GetOwner());
			Destroy();
		}
		UE_LOG(LogTemp, Display, TEXT("Actor Name : %s"), *OtherActor->GetActorNameOrLabel());
	}
}

void AStaffDefaultAttackProjectile::Init(const FVector& InMoveDirection)
{
	MoveDirection = InMoveDirection;
}
