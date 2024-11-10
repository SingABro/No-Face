// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyMelee_Common.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Stat/EnemyStatComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"
#include "AI/Controller/AIControllerCommon.h"
#include "UI/EnemyHpBarWidget.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Perception/AISense_Damage.h"

AEnemyMelee_Common::AEnemyMelee_Common()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Melee_Dusk.Minion_Lane_Melee_Dusk'"));
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	
	/* 콜리전 설정 */
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	GetCharacterMovement()->MaxWalkSpeed = 350.f;

	Stat->OnHpZero.AddUObject(this, &AEnemyMelee_Common::SetDead);

}

void AEnemyMelee_Common::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemyMelee_Common::AttackByAI()
{
	Super::AttackByAI();

	BeginAttack();
}

void AEnemyMelee_Common::DefaultAttackHitCheck()
{
	Super::DefaultAttackHitCheck();

	const float Damage = Stat->GetCurrentDamage();
	const float Range = Stat->GetCurrentRange();
	const float Degree = 60.f;

	FColor Color = FColor::Red;
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
				FDamageEvent DamageEvent;
				OverlapResult.GetActor()->TakeDamage(Damage, DamageEvent, GetController(), this);
				Color = FColor::Green;
			}
		}
	}
	
	DefaultAttackHitDebug(Origin, GetActorForwardVector(), Range, Degree, Color);

	if (!AttackInRange())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		
		AnimInstance->Montage_Stop(0.5f, DefaultAttackMontage);
	}
}

float AEnemyMelee_Common::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, FName Type)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser, Type);

	/* Hit Montage가 먼저 실행 되어야지 Dead 애니메이션이 잘 실행됨 */
	ImpactParticleComponent->SetTemplate(HitParticleCollection[Type]);
	BeginHitAction();

	Stat->ApplyDamage(Damage);

	/* 대미지 입으면 감각 활성화 */
	UAISense_Damage::ReportDamageEvent(
		GetWorld(),
		this,
		DamageCauser,
		Damage,
		GetActorLocation(),
		(GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal()
	);


	return Damage;
}

float AEnemyMelee_Common::TakeExp()
{
	//일반 몬스터 30 경험치
	return 30.0f;
}

void AEnemyMelee_Common::Stun()
{
	Super::Stun();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	GetMyController()->StopAI();
	AnimInstance->Montage_Play(StunMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyMelee_Common::EndStun);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, StunMontage);
}

void AEnemyMelee_Common::SetDead()
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

void AEnemyMelee_Common::BeginAttack()
{
	UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(DefaultAttackMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyMelee_Common::EndAttack);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, DefaultAttackMontage);
}

void AEnemyMelee_Common::EndAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	EnemyAttackFinished.ExecuteIfBound();
}

bool AEnemyMelee_Common::AttackInRange()
{
	const float Range = GetAttackInRange();
	FVector Origin = GetActorLocation();
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	return GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Range), Params);
}

void AEnemyMelee_Common::BeginHitAction()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	/* 스턴 상태라면 그대로 진행 */
	if (AnimInstance->Montage_IsPlaying(StunMontage))
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
	
	/* 피격 몽타주 실행 중 공격 금지 */
	GetMyController()->StopAI();

	ImpactParticleComponent->Activate();
	AnimInstance->Montage_Play(HitMontage, 0.5f);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AEnemyMelee_Common::EndHitAction);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, HitMontage);
}

void AEnemyMelee_Common::EndHitAction(UAnimMontage* Target, bool IsProperlyEnded)
{
	GetMyController()->RunAI();
}

void AEnemyMelee_Common::EndStun(UAnimMontage* Target, bool IsProperlyEnded)
{
	GetMyController()->RunAI();
}

bool AEnemyMelee_Common::IsInDegree(AActor* Actor, AActor* Target, float RadialAngle)
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

void AEnemyMelee_Common::DefaultAttackHitDebug(const FVector& Start, const FVector& ForwardVector, const float AttackRange, const float AttackDegree, const FColor& Color)
{
	// 부채꼴의 두 끝점 계산
	FVector LeftVector = ForwardVector.RotateAngleAxis(-AttackDegree / 2.0f, FVector::UpVector);
	FVector RightVector = ForwardVector.RotateAngleAxis(AttackDegree / 2.0f, FVector::UpVector);

	FVector LeftEndpoint = Start + LeftVector * AttackRange;
	FVector RightEndpoint = Start + RightVector * AttackRange;

	// 부채꼴의 중심선
	DrawDebugLine(GetWorld(), Start, Start + ForwardVector * AttackRange, Color, false, 3.0f);

	// 부채꼴의 두 끝선
	DrawDebugLine(GetWorld(), Start, LeftEndpoint, Color, false, 3.0f);
	DrawDebugLine(GetWorld(), Start, RightEndpoint, Color, false, 3.0f);
}

AAIControllerCommon* AEnemyMelee_Common::GetMyController()
{
	return CastChecked<AAIControllerCommon>(GetController());
}
