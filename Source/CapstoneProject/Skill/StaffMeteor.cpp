// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/StaffMeteor.h"
#include "Components/BoxComponent.h"
#include "Stat/CharacterDataStat.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

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

void AStaffMeteor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("충돌"));

	ParticleComponent->Activate();
	MeshComponent->SetHiddenInGame(true);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), 500.f, GetActorLocation(), 300.f, UDamageType::StaticClass(), TArray<AActor*>(), this);
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

