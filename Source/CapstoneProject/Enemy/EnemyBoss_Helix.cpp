// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBoss_Helix.h"
#include "Components/CapsuleComponent.h"
#include "AI/Controller/AIControllerHelix.h"
#include "Stat/EnemyStatComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "MotionWarpingComponent.h"

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


}

void AEnemyBoss_Helix::DefaultAttackHitCheck()
{
	Super::DefaultAttackHitCheck();

}

void AEnemyBoss_Helix::Skill1ByAI()
{
	Super::Skill1ByAI();

	BeginSkillDash();
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

void AEnemyBoss_Helix::BeginSkillDash()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	DashMotionWarpSet();
	AnimInstance->Montage_Play(DashMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyBoss_Helix::EndSkillDash);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, DashMontage);
}

void AEnemyBoss_Helix::EndSkillDash(UAnimMontage* Target, bool IsProperlyEnded)
{
	EnemySkill1Finished.ExecuteIfBound();
	MotionWarpingComponent->RemoveAllWarpTargets();
}

void AEnemyBoss_Helix::DashMotionWarpSet()
{
	FVector TargetOrigin = Player->GetActorLocation();
	FVector Origin = GetActorLocation();
	FVector TargetDir = (TargetOrigin - Origin);
	FVector Result = TargetDir.GetSafeNormal() * TargetDir.Size();
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(TEXT("Dash"), Result);
}

AAIControllerHelix* AEnemyBoss_Helix::GetMyController()
{
	return CastChecked<AAIControllerHelix>(GetController());
}
