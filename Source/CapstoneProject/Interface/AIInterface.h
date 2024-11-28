// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

DECLARE_DELEGATE(FEnemyAttackFinished)
DECLARE_DELEGATE(FEnemySkill1Finished)
DECLARE_DELEGATE(FEnemySkill2Finished)
DECLARE_DELEGATE(FEnemySkill3Finished)
DECLARE_DELEGATE(FEnemySkill4Finished)


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CAPSTONEPROJECT_API IAIInterface
{
	GENERATED_BODY()

public:
	virtual float GetPatrolRadius() = 0;
	virtual float GetDetectRadius() = 0;
	virtual float GetAttackInRange() = 0;
	virtual float GetTurnSpeed() = 0;
	virtual float GetDetectTime() = 0;

	virtual void SetEnemyAttackDelegate(const FEnemyAttackFinished& InEnemyAttackFinished) = 0;
	virtual void SetEnemySkill1Delegate(const FEnemySkill1Finished& InEnemySkill1Finished) = 0;
	virtual void SetEnemySkill2Delegate(const FEnemySkill1Finished& InEnemySkill2Finished) {};
	virtual void SetEnemySkill3Delegate(const FEnemySkill1Finished& InEnemySkill3Finished) {};
	virtual void SetEnemySkill4Delegate(const FEnemySkill1Finished& InEnemySkill4Finished) {};
	virtual void AttackByAI() = 0;
	virtual void DefaultAttackHitCheck() = 0;
	virtual void Skill_1_HitCheck() {};

	virtual void Skill1ByAI() = 0;
	virtual void Skill2ByAI() {};
	virtual void Skill3ByAI() {};
	virtual void Skill4ByAI() {};
};
