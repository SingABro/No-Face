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
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

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

float AEnemyBoss_Helix::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

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


}

void AEnemyBoss_Helix::BeginDefaultAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	Skill_1_MotionWarpSet();
	AnimInstance->Montage_Play(DefaultAttackMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyBoss_Helix::EndDefaultAttack);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, DefaultAttackMontage);
}

void AEnemyBoss_Helix::EndDefaultAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	EnemyAttackFinished.ExecuteIfBound();
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
}

void AEnemyBoss_Helix::BeginSkill_2()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

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

	const float Damage = 300.f;
	const float Range = 700.f;

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	FColor Color = FColor::Red;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TargetLoc, TargetLoc + TargetRoc.Vector() * Range, ECC_GameTraceChannel1, Params);
	if (bHit)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(Damage, DamageEvent, GetController(), this);
		Color = FColor::Green;
	}
	DrawDebugLine(GetWorld(), TargetLoc, TargetLoc + TargetRoc.Vector() * Range, Color, false, 3.f);

}

AAIControllerHelix* AEnemyBoss_Helix::GetMyController()
{
	return CastChecked<AAIControllerHelix>(GetController());
}
