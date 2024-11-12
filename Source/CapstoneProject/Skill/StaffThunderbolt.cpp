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
}

void AStaffThunderbolt::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStaffThunderbolt::ActiveThunderbolt()
{
	TArray<FOverlapResult> OverlapResults;

	if (CheckInArea(OverlapResults))
	{
		FDamageEvent DamageEvent;
		for (const auto& OverlapResult : OverlapResults)
		{
			AEnemyBase* Enemy = Cast<AEnemyBase>(OverlapResult.GetActor());
			if (Enemy)
			{
				if (GetOwner() == nullptr) return;
				Enemy->TakeDamage(Stat->Staff_R_Damage + Stat->Staff_R_Level * 300.f, DamageEvent, GetWorld()->GetFirstPlayerController(), GetOwner(), TEXT("Default"));
			}
		}
		DrawDebugSphere(GetWorld(), GetActorLocation(), Stat->Staff_R_Range, 32, FColor::Green, false, 3.f);
	}
}

bool AStaffThunderbolt::CheckInArea(TArray<FOverlapResult>& InOverlapResults)
{
	FVector Origin = GetActorLocation();
	FCollisionQueryParams Params(NAME_None, true, GetOwner()); //GetOwner 꼭 설정해주기

	return GetWorld()->OverlapMultiByChannel(InOverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Stat->Staff_R_Range), Params);
}

void AStaffThunderbolt::ThunderboltDestory(UParticleSystemComponent* PSystem)
{
	Destroy();
}

