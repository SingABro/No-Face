// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/StaffThunderbolt.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"
#include "Stat/CharacterDataStat.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

AStaffThunderbolt::AStaffThunderbolt()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleComponent->SetupAttachment(Root);
	ParticleComponent->SetTemplate(Particle);
	ParticleComponent->OnSystemFinished.AddDynamic(this, &AStaffThunderbolt::ThunderboltDestory);

	Damage = Stat->Staff_R_Damage;
}

void AStaffThunderbolt::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStaffThunderbolt::ActiveThunderbolt()
{
	if (CheckInArea())
	{
		FDamageEvent DamageEvent;
		for (const auto& OverlapResult : OverlapResults)
		{
			OverlapResult.GetActor()->TakeDamage(Damage, DamageEvent, OverlapResult.GetActor()->GetInstigatorController(), GetOwner());
		}
		DrawDebugSphere(GetWorld(), GetActorLocation(), 350.f, 32, FColor::Green, false, 3.f);
	}
}

bool AStaffThunderbolt::CheckInArea()
{
	const float Radius = 350.f;

	FVector Origin = GetActorLocation();
	FCollisionQueryParams Params(NAME_None, true, GetOwner()); //GetOwner 꼭 설정해주기

	return GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius), Params);
}

void AStaffThunderbolt::ThunderboltDestory(UParticleSystemComponent* PSystem)
{
	Destroy();
}

