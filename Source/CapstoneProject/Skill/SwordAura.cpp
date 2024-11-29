// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SwordAura.h"
#include "Skill/SkillComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "Stat/CharacterDataStat.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

ASwordAura::ASwordAura()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->OnComponentBeginOverlap.AddDynamic(this, &ASwordAura::OnBeginOverlap);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Paticle Component"));
	ParticleComponent->SetupAttachment(Box);
	ParticleComponent->SetTemplate(Particle);

	MoveDirection = FVector::ZeroVector;
	MoveSpeed = 2000.f;
	LifeTime = 2.5f;

	Color = FColor::Red;
}

void ASwordAura::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASwordAura::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector NewLocation = GetActorLocation() + (MoveDirection * MoveSpeed * DeltaTime);
	SetActorLocation(NewLocation);

	LifeTime -= DeltaTime;
	if (LifeTime <= 0.f)
	{
		Destroy();	//LifeTime 동안 검기가 움직이도록 설정
	}

	DrawDebugBox(GetWorld(), GetActorLocation(), Box->GetScaledBoxExtent(), Color, false);
}

void ASwordAura::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		FDamageEvent DamageEvent;
		AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
		if (Enemy)
		{
			Enemy->TakeDamage(Stat->Sword_Q_Damage + Stat->Sword_Q_Level * 120, DamageEvent, GetWorld()->GetFirstPlayerController(), this, TEXT("Sword_Q"));
		}
		Color = FColor::Green;
	}
}

void ASwordAura::Init(const FVector& InMoveDirection)
{
	MoveDirection = InMoveDirection;
}



