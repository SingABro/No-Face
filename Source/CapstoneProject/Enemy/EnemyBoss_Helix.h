// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "Interface/HelixSkillInterface.h"
#include "EnemyBoss_Helix.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API AEnemyBoss_Helix : public AEnemyBase, public IHelixSkillInterface
{
	GENERATED_BODY()
	
public:
	AEnemyBoss_Helix();

protected:
	virtual void BeginPlay() override;

public:
	/* AI 인터페이스 구현 섹션 */
	virtual void AttackByAI() override;
	virtual void DefaultAttackHitCheck() override;
	virtual float GetAttackInRange() override;

	virtual void Skill1ByAI() override;
	virtual void Skill2ByAI() override;
	virtual void Skill3ByAI() override;
	virtual void Skill4ByAI() override;

	virtual void SetEnemySkill2Delegate(const FEnemySkill1Finished& InEnemySkill2Finished) override;
	virtual void SetEnemySkill3Delegate(const FEnemySkill1Finished& InEnemySkill3Finished) override;
	virtual void SetEnemySkill4Delegate(const FEnemySkill1Finished& InEnemySkill4Finished) override;

	FEnemySkill2Finished EnemySkill2Finished;
	FEnemySkill3Finished EnemySkill3Finished;
	FEnemySkill4Finished EnemySkill4Finished;


	/* 오버라이딩 섹션 */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeExp() override;
	virtual void Stun() override;
	virtual void SetDead() override;


private:
	void BeginDefaultAttack();
	void EndDefaultAttack(class UAnimMontage* Target, bool IsProperlyEnded);

	/* Dash 스킬 */
	void BeginSkill_1();
	void EndSkill_1(class UAnimMontage* Target, bool IsProperlyEnded);
	void Skill_1_MotionWarpSet();
	virtual void Skill_1_HitCheck() override;

	/* 크아앙 스킬 */
	void BeginSkill_2();
	void EndSkill_2(class UAnimMontage* Target, bool IsProperlyEnded);
	virtual void Skill_2_HitCheck() override;

	/* 땅 퍽 치는 스킬 */
	void BeginSkill_3();
	void EndSkill_3(class UAnimMontage* Target, bool IsProperlyEnded);
	virtual void Skill_3_HitCheck() override;

	/* 레이저 스킬 */
	void BeginSkill_4();
	void EndSkill_4(class UAnimMontage* Target, bool IsProperlyEnded);
	virtual void Skill_4_HitCheck() override;


/* 유틸리티 */
private:
	class AAIControllerHelix* GetMyController();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> Player;

/* 모션 워핑 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Warp")
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;


/* 몽타주 */
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> Skill_1Montage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> Skill_2Montage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> Skill_3Montage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> Skill_4Montage;

/* 파티클 */
private:
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystem> DefaultAttackEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystem> Skill_1Effect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystem> Skill_2Effect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystem> Skill_3Effect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystem> Skill_4Effect;
};
