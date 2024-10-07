// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "EnemyBoss_Helix.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API AEnemyBoss_Helix : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	AEnemyBoss_Helix();

public:
	/* AI 인터페이스 구현 섹션 */
	virtual void AttackByAI() override;
	virtual void DefaultAttackHitCheck() override;

	virtual void Skill1ByAI() override;

	/* 오버라이딩 섹션 */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeExp() override;
	virtual void Stun() override;
	virtual void SetDead() override;

};
