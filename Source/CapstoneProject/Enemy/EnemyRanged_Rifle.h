// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "EnemyRanged_Rifle.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API AEnemyRanged_Rifle : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyRanged_Rifle();

public:
	/* AI 인터페이스 구현 섹션 */
	virtual void AttackByAI() override;
	virtual void DefaultAttackHitCheck() override;
	virtual float GetPatrolRadius() override;
	virtual float GetDetectRadius() override;
	virtual float GetAttackInRange() override;

	/* 오버라이딩 섹션 */
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, FName Type) override;
	virtual float TakeExp() override;
	virtual void Stun() override;
	virtual void SetDead() override;

	void SetWalkSpeed();
	void SetRunSpeed();

private:
	void BeginAttack();
	void EndAttack(class UAnimMontage* Target, bool IsProperlyEnded);

	void BeginMeleeAttack();
	void EndMeleeAttack(class UAnimMontage* Target, bool IsProperlyEnded);

	void BeginHitAction();
	void EndHitAction(class UAnimMontage* Target, bool IsProperlyEnded);

	/* 스턴 애니메이션 끝날 때 실행되는 함수 */
	void EndStun(class UAnimMontage* Target, bool IsProperlyEnded);

/* 발사체 */
private:
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<class AEnemyRangedProjectile> ProjectileClass;

/* 유틸리티 섹션 */
private:
	class AAIControllerRifle* GetMyController();

/* 이펙트 섹션 */
private:
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystem> FireEffect;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystemComponent> FireEffectComponent;

/* 몽타주 섹션 */
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> MeleeAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> StunMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> Skill1Montage;
};
