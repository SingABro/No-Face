// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/StaffMeteor.h"
#include "Components/BoxComponent.h"
#include "Stat/CharacterDataStat.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"

AStaffMeteor::AStaffMeteor()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->OnComponentBeginOverlap.AddDynamic(this, &AStaffMeteor::OnBeginOverlap);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(Box);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ParagonGideon/FX/Meshes/Heroes/Gideon/Abilities/SM_Meteor_Rot.SM_Meteor_Rot'"));
	if (MeshRef.Object)
	{
		MeshComponent->SetStaticMesh(MeshRef.Object);
	}

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Paticle"));
	ParticleComponent->SetupAttachment(Box);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> CrashEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/No-Face/Effect/Staff/Staff_Q/Staff_Q_MeteorCrash.Staff_Q_MeteorCrash'"));
	if (CrashEffectRef.Object)
	{
		CrashEffect = CrashEffectRef.Object;
		ParticleComponent->SetTemplate(CrashEffect);
	}
	ParticleComponent->bAutoActivate = false;
	ParticleComponent->OnSystemFinished.AddDynamic(this, &AStaffMeteor::MeteorDestroy);

	MoveSpeed = 300.f;
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

void AStaffMeteor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ParticleComponent->Activate();
	MeshComponent->SetHiddenInGame(true);

	TArray<FOverlapResult> OverlapResults;
	FVector Origin = GetActorLocation();
	FCollisionQueryParams Params(NAME_None, true, GetOwner()); //GetOwner 꼭 설정해주기

	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Stat->Staff_Q_Range), Params);
	if (bHit)
	{
		FDamageEvent DamageEvent;
		for (const auto& OverlapResult : OverlapResults)
		{
			AEnemyBase* Enemy = Cast<AEnemyBase>(OverlapResult.GetActor());
			if (Enemy)
			{
				if (GetOwner() == nullptr) return;
				Enemy->TakeDamage(Stat->Staff_Q_Damage, DamageEvent, GetWorld()->GetFirstPlayerController(), GetOwner(), TEXT("Default"));
			}
		}
		DrawDebugSphere(GetWorld(), GetActorLocation(), Stat->Staff_Q_Range, 32, FColor::Green, false, 3.f);
	}
}

void AStaffMeteor::MeteorDestroy(UParticleSystemComponent* PSystem)
{
	Destroy();
}

void AStaffMeteor::Init(const FVector& InDestination)
{
	Destination = InDestination;
	bStart = true;
}

