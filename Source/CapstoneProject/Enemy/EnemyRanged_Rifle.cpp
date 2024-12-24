// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyRanged_Rifle.h"
#include "Components/CapsuleComponent.h"
#include "Stat/EnemyStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/Controller/AIControllerRifle.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapon/EnemyRangedProjectile.h"

AEnemyRanged_Rifle::AEnemyRanged_Rifle()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Ranged_Dusk.Minion_Lane_Ranged_Dusk'"));
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	/* 콜리전 설정 */
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	/* 죽을 때 */
	Stat->OnHpZero.AddUObject(this, &AEnemyRanged_Rifle::SetDead);

	/* 파티클 설정 */
	FireEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire Effect"));
	FireEffectComponent->SetupAttachment(GetMesh(), TEXT("Muzzle_02"));
	FireEffectComponent->bAutoActivate = false;

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void AEnemyRanged_Rifle::AttackByAI()
{
	Super::AttackByAI();
	
	if (GetMyController()->CanMeleeAttack())
	{
		BeginMeleeAttack();
	}
	else
	{
		BeginAttack();
	}
}

void AEnemyRanged_Rifle::DefaultAttackHitCheck()
{
	Super::DefaultAttackHitCheck();

	float Damage = 150.f;
	float Degree = 60.f;

	FVector Origin = GetActorLocation();
	FCollisionQueryParams Param(NAME_None, false, this);

	if (GetMyController()->CanMeleeAttack())
	{
		TArray<FOverlapResult> OverlapResults;
		if (GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(400.f), Param))
		{
			for (const FOverlapResult& OverlapResult : OverlapResults)
			{
				AActor* Target = OverlapResult.GetActor();
				FVector PlayerLocation = GetActorLocation();
				FVector TargetLocation = Target->GetActorLocation();
				FVector ForwardVector = GetActorForwardVector();
				FVector DirectionToTarget = (TargetLocation - PlayerLocation).GetSafeNormal();

				float DotProduct = FVector::DotProduct(ForwardVector, DirectionToTarget);
				float AngleToTarget = FMath::Acos(DotProduct);
				float AngleToTargetDegrees = FMath::RadiansToDegrees(AngleToTarget);

				FDamageEvent DamageEvent;
				if (AngleToTargetDegrees <= (Degree / 2.0f))
				{
					Target->TakeDamage(Damage, DamageEvent, GetController(), this);
				}
			}
		}
		else
		{
			UAnimInstance* Anim = GetMesh()->GetAnimInstance();
			Anim->Montage_Stop(1.f, MeleeAttackMontage);
			float RestTime = MeleeAttackMontage->GetPlayLength() - Anim->Montage_GetPosition(MeleeAttackMontage);

			FTimerHandle MeleeTimer;
			GetWorld()->GetTimerManager().SetTimer(MeleeTimer, [&]()
				{
					EnemyAttackFinished.ExecuteIfBound();
				}, RestTime, false);
		}

		return;
	}

	FVector SpawnLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_02")) + GetActorForwardVector() * 50.f;
	FRotator SpawnRot = GetMesh()->GetSocketRotation(TEXT("Muzzle_02"));
	AEnemyRangedProjectile* Projectile = GetWorld()->SpawnActor<AEnemyRangedProjectile>(ProjectileClass, SpawnLoc, SpawnRot);
	Projectile->SetOwner(this);
	Projectile->Init(GetActorForwardVector());
}

float AEnemyRanged_Rifle::GetPatrolRadius()
{
	return 500.f;
}

float AEnemyRanged_Rifle::GetDetectRadius()
{
	return 2000.f;
}

float AEnemyRanged_Rifle::GetAttackInRange()
{
	return 2000.f;
}

float AEnemyRanged_Rifle::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, FName Type)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser, Type);

	ImpactParticleComponent->SetTemplate(HitParticleCollection[Type]);
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance->Montage_IsPlaying(DefaultAttackMontage))
	{
		BeginHitAction();
	}

	Stat->ApplyDamage(Damage);


	return Damage;
}

float AEnemyRanged_Rifle::TakeExp()
{
	Super::TakeExp();
	
	return 50.0f;
}

void AEnemyRanged_Rifle::Stun()
{
	Super::Stun();

	UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());

	GetMyController()->StopAI();
	AnimInstance->Montage_Play(StunMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyRanged_Rifle::EndStun);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, StunMontage);
}

void AEnemyRanged_Rifle::SetDead()
{
	Super::SetDead();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->StopAllMontages(5.f);
	AnimInstance->Montage_Play(DeadMontage);
	
	GetMyController()->StopAI();

	SetActorEnableCollision(false);
}

void AEnemyRanged_Rifle::SetWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void AEnemyRanged_Rifle::SetRunSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void AEnemyRanged_Rifle::BeginAttack()
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	Anim->Montage_Play(DefaultAttackMontage);
	FireEffectComponent->Activate();

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyRanged_Rifle::EndAttack);
	Anim->Montage_SetEndDelegate(MontageEnd, DefaultAttackMontage);
}

void AEnemyRanged_Rifle::EndAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	FireEffectComponent->Deactivate();
	EnemyAttackFinished.ExecuteIfBound();
}

void AEnemyRanged_Rifle::BeginMeleeAttack()
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	Anim->Montage_Play(MeleeAttackMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyRanged_Rifle::EndMeleeAttack);
	Anim->Montage_SetEndDelegate(MontageEnd, MeleeAttackMontage);
}

void AEnemyRanged_Rifle::EndMeleeAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	EnemyAttackFinished.ExecuteIfBound();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AEnemyRanged_Rifle::BeginHitAction()
{
	///* 피격 몽타주 실행 중 공격 금지 */
	//GetMyController()->StopAI();

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

	/* 파티클도 바로바로 재시작 */
	if (ImpactParticleComponent->IsActive())
	{
		ImpactParticleComponent->Deactivate();
	}

	AnimInstance->Montage_Play(HitMontage);
	ImpactParticleComponent->Activate();

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyRanged_Rifle::EndHitAction);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, HitMontage);
}

void AEnemyRanged_Rifle::EndHitAction(UAnimMontage* Target, bool IsProperlyEnded)
{
	if (!IsDead)
	{
		GetMyController()->RunAI();
	}
}

void AEnemyRanged_Rifle::EndStun(UAnimMontage* Target, bool IsProperlyEnded)
{
	GetMyController()->RunAI();
}

AAIControllerRifle* AEnemyRanged_Rifle::GetMyController()
{
	return CastChecked<AAIControllerRifle>(GetController());
}
