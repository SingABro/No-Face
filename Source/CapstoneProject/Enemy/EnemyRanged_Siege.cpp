// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyRanged_Siege.h"
#include "Components/CapsuleComponent.h"
#include "Stat/EnemyStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/Controller/AIControllerSiege.h"
#include "Engine/DamageEvents.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

AEnemyRanged_Siege::AEnemyRanged_Siege()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Siege_Dusk.Minion_Lane_Siege_Dusk'"));
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	/* 콜리전 설정 */
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	/* 죽을 때 */
	Stat->OnHpZero.AddUObject(this, &AEnemyRanged_Siege::SetDead);
}

void AEnemyRanged_Siege::AttackByAI()
{
	Super::AttackByAI();

	BeginAttack();
}

void AEnemyRanged_Siege::DefaultAttackHitCheck()
{
	Super::DefaultAttackHitCheck();

	float Damage = 50.f;
	float Range = 1000.f;

	FVector Origin = GetActorLocation();
	FVector End = Origin + GetActorForwardVector() * Range;
	FCollisionQueryParams Param(NAME_None, false, this);
	FHitResult HitResult;

	FVector ForwardVector = GetActorForwardVector() * Range;
	FQuat RootRot = FRotationMatrix::MakeFromZ(ForwardVector).ToQuat();
	FVector BoxExtent = FVector(100.f, 100.f, 100.f);

	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Origin, End, RootRot, ECC_GameTraceChannel1, FCollisionShape::MakeBox(BoxExtent), Param);
	if (bHit)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(Damage, DamageEvent, GetController(), this);
	}

	DrawDebugBox(GetWorld(), Origin, BoxExtent, FColor::Green, false, 3.f);
	DrawDebugBox(GetWorld(), End, BoxExtent, FColor::Green, false, 3.f);
}

float AEnemyRanged_Siege::GetDetectRadius()
{
	return 2000.f;
}

float AEnemyRanged_Siege::GetAttackInRange()
{
	return 1500.f;
}

void AEnemyRanged_Siege::Skill1ByAI()
{
	Super::Skill1ByAI();

	BeginSkill1();
}

float AEnemyRanged_Siege::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	BeginHitAction();

	Stat->ApplyDamage(Damage);

	return Damage;
}

float AEnemyRanged_Siege::TakeExp()
{
	Super::TakeExp();

	return 4321.0f;
}

void AEnemyRanged_Siege::Stun()
{
	Super::Stun();

	UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(StunMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyRanged_Siege::EndStun);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, StunMontage);
}

void AEnemyRanged_Siege::SetDead()
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

void AEnemyRanged_Siege::BeginAttack()
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	Anim->Montage_Play(DefaultAttackMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyRanged_Siege::EndAttack);
	Anim->Montage_SetEndDelegate(MontageEnd, DefaultAttackMontage);
}

void AEnemyRanged_Siege::EndAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	EnemyAttackFinished.ExecuteIfBound();
}

void AEnemyRanged_Siege::BeginHitAction()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	/* 스턴 상태라면 그대로 진행 */
	if (AnimInstance->Montage_IsPlaying(StunMontage) || AnimInstance->Montage_IsPlaying(Skill1Montage))
	{
		return;
	}

	/* 만약 몽타주 실행 중 한번 더 맞는다면 멈추고 빠른 재시작 */
	if (AnimInstance->Montage_IsPlaying(HitMontage))
	{
		AnimInstance->Montage_Stop(0.1f, HitMontage);
	}

	/* 피격 몽타주 실행 중 공격 금지 */
	GetMyController()->StopAI();

	AnimInstance->Montage_Play(HitMontage);
	ImpactParticleComponent->Activate();

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyRanged_Siege::EndHitAction);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, HitMontage);
}

void AEnemyRanged_Siege::EndHitAction(UAnimMontage* Target, bool IsProperlyEnded)
{
	GetMyController()->RunAI();
}

void AEnemyRanged_Siege::BeginSkill1()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(Skill1Montage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyRanged_Siege::EndSkill1);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, Skill1Montage);
}

void AEnemyRanged_Siege::EndSkill1(UAnimMontage* Target, bool IsProperlyEnded)
{
	EnemySkill1Finished.ExecuteIfBound();
}

void AEnemyRanged_Siege::EndStun(UAnimMontage* Target, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

AAIControllerSiege* AEnemyRanged_Siege::GetMyController()
{
	return CastChecked<AAIControllerSiege>(GetController());
}

