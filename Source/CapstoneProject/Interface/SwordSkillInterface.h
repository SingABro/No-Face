// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SwordSkillInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USwordSkillInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CAPSTONEPROJECT_API ISwordSkillInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Sword_Q_SkillHitCheck() = 0;
	virtual void Sword_W_SkillHitCheck() = 0;
	virtual void Sword_E_SkillHitCheck() = 0;
	virtual void Sword_R_SkillHitCheck() = 0;

};
