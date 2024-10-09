// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/StaffMeteor.h"
#include "Components/BoxComponent.h"
#include "Stat/CharacterDataStat.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

AStaffMeteor::AStaffMeteor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Root);
	Box->OnComponentHit.AddDynamic(this, &AStaffMeteor::OnHit);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Paticle"));
	ParticleComponent->SetupAttachment(Root);
	ParticleComponent->SetTemplate(Particle);

	MoveSpeed = 100.f;
	Damage = Stat->Staff_Q_Damage;
	Destination = FVector::ZeroVector;
	bStart = false;
}

void AStaffMeteor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStaffMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStart)
	{
		FVector Origin = GetActorLocation();
		SetActorLocation(FMath::VInterpConstantTo(Origin, Destination, DeltaTime, MoveSpeed));
	}
}

void AStaffMeteor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("충돌"));
	Destroy();

}

void AStaffMeteor::Init(const FVector& InDestination)
{
	Destination = InDestination;
	bStart = true;
}

