// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/StaffArea.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"
#include "Stat/CharacterDataStat.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

AStaffArea::AStaffArea()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleComponent->SetupAttachment(Root);
	ParticleComponent->SetTemplate(Particle);
	ParticleComponent->OnSystemFinished.AddDynamic(this, &AStaffArea::AreaDestroy);

	DamageTime = 0.f;
}

void AStaffArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStaffArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PullToCenter(DeltaTime);
}

bool AStaffArea::CheckInArea(TArray<FOverlapResult>& InOverlapResults)
{
	FVector Origin = GetActorLocation();
	FCollisionQueryParams Params(NAME_None, true, GetOwner()); //GetOwner 꼭 설정해주기

	return GetWorld()->OverlapMultiByChannel(InOverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Stat->Staff_W_Range), Params);
}

void AStaffArea::PullToCenter(float DeltaTime)
{
	FDamageEvent DamageEvent;
	TArray<FOverlapResult> OverlapResults;

	if (CheckInArea(OverlapResults))
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			FVector Target = GetActorLocation();
			AActor* Actor = OverlapResult.GetActor();
			Target.Z = Actor->GetActorLocation().Z;
			Actor->SetActorLocation(FMath::VInterpTo(Actor->GetActorLocation(), Target, DeltaTime, 1.f));

			DamageTime += DeltaTime;
			if (DamageTime >= 1.f)
			{
				AEnemyBase* Enemy = Cast<AEnemyBase>(OverlapResult.GetActor());
				if (Enemy)
				{
					if (GetOwner() == nullptr) return;
					Enemy->TakeDamage(Stat->Staff_W_Damage, DamageEvent, GetWorld()->GetFirstPlayerController(), GetOwner(), TEXT("Default"));
				}
				DamageTime = 0.f;
			}
			DrawDebugSphere(GetWorld(), GetActorLocation(), Stat->Staff_W_Range, 32, FColor::Green, false);
		}
	}
	
}

void AStaffArea::AreaDestroy(UParticleSystemComponent* PSystem)
{
	Destroy();
}

