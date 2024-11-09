// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HelixSkillInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHelixSkillInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CAPSTONEPROJECT_API IHelixSkillInterface
{
	GENERATED_BODY()

public:
	virtual void Skill_1_HitCheck() = 0;
	virtual void Skill_2_HitCheck() = 0;
	virtual void Skill_3_HitCheck() = 0;
	virtual void Skill_4_HitCheck() = 0;
};
