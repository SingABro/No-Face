// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBoss_Helix.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/Controller/AIControllerHelix.h"
#include "Stat/EnemyStatComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "MotionWarpingComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"

AEnemyBoss_Helix::AEnemyBoss_Helix()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Prime_Helix/Meshes/Prime_Helix.Prime_Helix'"));
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	/* 콜리전 설정 */
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	Stat->OnHpZero.AddUObject(this, &AEnemyBoss_Helix::SetDead);

	/* 모션 워핑 */
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warp"));

}

void AEnemyBoss_Helix::BeginPlay()
{
	Super::BeginPlay();

	Player = GetMyController()->GetTarget();
}

void AEnemyBoss_Helix::AttackByAI()
{
	Super::AttackByAI();

	BeginDefaultAttack();
}

void AEnemyBoss_Helix::DefaultAttackHitCheck()
{
	Super::DefaultAttackHitCheck();

	const float Damage = Stat->GetCurrentDamage();
	const float Range = Stat->GetCurrentRange();
	const float Degree = 150.f;

	FVector Origin = GetActorLocation();
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Range), Params);
	if (bHit)
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			if (IsInDegree(this, OverlapResult.GetActor(), Degree))
			{
				float Distance = FVector::Distance(Origin, OverlapResult.GetActor()->GetActorLocation());
				FDamageEvent DamageEvent;
				OverlapResult.GetActor()->TakeDamage(Damage - (Distance * 0.3f), DamageEvent, GetController(), this);
			}
		}
	}
}

float AEnemyBoss_Helix::GetAttackInRange()
{
	Super::GetAttackInRange();

	return 300.f;
}

void AEnemyBoss_Helix::Skill1ByAI()
{
	Super::Skill1ByAI();

	BeginSkill_1();
}

void AEnemyBoss_Helix::Skill2ByAI()
{
	Super::Skill2ByAI();

	BeginSkill_2();
}

void AEnemyBoss_Helix::Skill3ByAI()
{
	Super::Skill3ByAI();

	BeginSkill_3();
}

void AEnemyBoss_Helix::Skill4ByAI()
{
	Super::Skill4ByAI();

	BeginSkill_4();
}

void AEnemyBoss_Helix::SetEnemySkill2Delegate(const FEnemySkill1Finished& InEnemySkill2Finished)
{
	EnemySkill2Finished = InEnemySkill2Finished;
}

void AEnemyBoss_Helix::SetEnemySkill3Delegate(const FEnemySkill1Finished& InEnemySkill3Finished)
{
	EnemySkill3Finished = InEnemySkill3Finished;
}

void AEnemyBoss_Helix::SetEnemySkill4Delegate(const FEnemySkill1Finished& InEnemySkill4Finished)
{
	EnemySkill4Finished = InEnemySkill4Finished;
}

float AEnemyBoss_Helix::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, FName Type)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser, Type);

	ImpactParticleComponent->SetTemplate(HitParticleCollection[Type]);
	ImpactParticleComponent->Activate();

	Stat->ApplyDamage(Damage);
	UE_LOG(LogTemp, Display, TEXT("실행됨?"));

	return 0.0f;
}

float AEnemyBoss_Helix::TakeExp()
{
	Super::TakeExp();

	return 0.0f;
}

void AEnemyBoss_Helix::Stun()
{
	Super::Stun();

}

void AEnemyBoss_Helix::SetDead()
{
	Super::SetDead();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->StopAllMontages(5.f);
	AnimInstance->Montage_Play(DeadMontage);

	GetMyController()->StopAI();

	SetActorEnableCollision(false);

	FTimerHandle DestroyHandle;
	GetWorld()->GetTimerManager().SetTimer(DestroyHandle,
		[&]()
		{
			Destroy();
		}, 4.f, false);
}

void AEnemyBoss_Helix::BeginDefaultAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(DefaultAttackMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyBoss_Helix::EndDefaultAttack);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, DefaultAttackMontage);
}

void AEnemyBoss_Helix::EndDefaultAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	EnemyAttackFinished.ExecuteIfBound();
}

bool AEnemyBoss_Helix::IsInDegree(AActor* Actor, AActor* Target, float RadialAngle)
{
	if (!Actor || !Target) return false;

	FVector PlayerLocation = Actor->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	FVector ForwardVector = Actor->GetActorForwardVector();
	FVector DirectionToTarget = (TargetLocation - PlayerLocation).GetSafeNormal();

	// 타겟이 부채꼴 각도 내에 있는지 확인
	float DotProduct = FVector::DotProduct(ForwardVector, DirectionToTarget);
	float AngleToTarget = FMath::Acos(DotProduct);

	// 라디안에서 각도로 변환
	float AngleToTargetDegrees = FMath::RadiansToDegrees(AngleToTarget);

	return AngleToTargetDegrees <= (RadialAngle / 2.0f);
}

void AEnemyBoss_Helix::BeginSkill_1()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	Skill_1_MotionWarpSet();
	AnimInstance->Montage_Play(Skill_1Montage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyBoss_Helix::EndSkill_1);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, Skill_1Montage);
}

void AEnemyBoss_Helix::EndSkill_1(UAnimMontage* Target, bool IsProperlyEnded)
{
	EnemySkill1Finished.ExecuteIfBound();
	MotionWarpingComponent->RemoveAllWarpTargets();
}

void AEnemyBoss_Helix::Skill_1_MotionWarpSet()
{
	FVector TargetOrigin = Player->GetActorLocation();
	FVector Origin = GetActorLocation();
	FVector TargetDir = (TargetOrigin - Origin);
	FRotator TargetRot = FRotationMatrix::MakeFromX(TargetDir).Rotator();
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("Dash"), TargetOrigin, TargetRot);
}

void AEnemyBoss_Helix::Skill_1_HitCheck()
{
	FVector Origin = GetActorLocation();
	FVector TargetLoc = Origin + GetActorForwardVector() * 300.f;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skill_1Effect, TargetLoc, GetActorRotation());

	float Damage = 800.f;
	float Range = 300.f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults, TargetLoc, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Range), Params);
	if (bHit)
	{
		FDamageEvent DamageEvent;
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			OverlapResult.GetActor()->TakeDamage(Damage, DamageEvent, GetController(), this);
		}
	}
}

void AEnemyBoss_Helix::BeginSkill_2()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	Skill2_Effect();
	AnimInstance->Montage_Play(Skill_2Montage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyBoss_Helix::EndSkill_2);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, Skill_2Montage);
}

void AEnemyBoss_Helix::EndSkill_2(UAnimMontage* Target, bool IsProperlyEnded)
{
	EnemySkill2Finished.ExecuteIfBound();
}

void AEnemyBoss_Helix::Skill_2_HitCheck()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skill_2Effect, GetActorLocation(), GetActorRotation());

	const float Damage = 1200.f;
	const float Range = 700.f;

	TArray<FOverlapResult> OverlapResults;
	FVector Origin = GetActorLocation();
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Range), Params);
	if (bHit)
	{
		FDamageEvent DamageEvent;
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			float Distance = FVector::Distance(OverlapResult.GetActor()->GetActorLocation(), Origin);
			OverlapResult.GetActor()->TakeDamage(Damage - (Distance * 0.3), DamageEvent, GetController(), this);
		}
	}
}

void AEnemyBoss_Helix::Skill2_Effect()
{
	FVector TargetLoc = GetMesh()->GetSocketLocation(TEXT("Chest"));
	FRotator TargetRoc = GetMesh()->GetSocketRotation(TEXT("Chest"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skill_2ChargeEffect, TargetLoc, TargetRoc);
}

void AEnemyBoss_Helix::BeginSkill_3()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(Skill_3Montage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyBoss_Helix::EndSkill_3);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, Skill_3Montage);
}

void AEnemyBoss_Helix::EndSkill_3(UAnimMontage* Target, bool IsProperlyEnded)
{
	EnemySkill3Finished.ExecuteIfBound();
}

void AEnemyBoss_Helix::Skill_3_HitCheck()
{
	FVector Origin = GetActorLocation();
	FVector TargetLoc = Origin + GetActorForwardVector() * 600.f;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skill_3Effect, TargetLoc, GetActorRotation());

	FQuat DirectionQuat = FRotationMatrix::MakeFromX(TargetLoc).ToQuat();
	
	float Damage = 600.f;
	float Range = 600.f;
	FHitResult HitResult;
	FVector EndHit = TargetLoc + GetActorForwardVector() * Range;
	FVector BoxExtent = FVector(1.f, 200.f, 100.f);
	FCollisionQueryParams Params(NAME_None, false, this);
	
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, TargetLoc, EndHit, DirectionQuat, ECC_GameTraceChannel1, FCollisionShape::MakeBox(BoxExtent), Params);
	if (bHit)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(Damage, DamageEvent, GetController(), this);
	}
}

void AEnemyBoss_Helix::BeginSkill_4()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(Skill_4Montage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyBoss_Helix::EndSkill_4);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, Skill_4Montage);
}

void AEnemyBoss_Helix::EndSkill_4(UAnimMontage* Target, bool IsProperlyEnded)
{
	EnemySkill4Finished.ExecuteIfBound();
}

void AEnemyBoss_Helix::Skill_4_HitCheck()
{
	FVector TargetLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_Front"));
	FRotator TargetRoc = GetMesh()->GetSocketRotation(TEXT("Muzzle_Front"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skill_4Effect, TargetLoc, TargetRoc);

	float Damage = 2000.f;
	float Range = 700.f;

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	FColor Color = FColor::Red;
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, TargetLoc, TargetLoc + TargetRoc.Vector() * Range, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeBox(FVector(1.f, 50.f, 50.f)), Params);
	if (bHit)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(Damage, DamageEvent, GetController(), this);
		Color = FColor::Green;
	}
	
	FVector BoxCenter = (TargetLoc + (TargetLoc + TargetRoc.Vector() * Range)) * 0.5f;
	FVector BoxExtent(1.f, 50.f, 50.f);
	DrawDebugBox(GetWorld(), BoxCenter, BoxExtent, FQuat(TargetRoc), Color, false, 2.f);
	DrawDebugLine(GetWorld(), TargetLoc, TargetLoc + TargetRoc.Vector() * Range, Color, false, 2.f, 0, 2.f);

}

AAIControllerHelix* AEnemyBoss_Helix::GetMyController()
{
	return CastChecked<AAIControllerHelix>(GetController());
}
