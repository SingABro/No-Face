// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StaffSkillInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStaffSkillInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CAPSTONEPROJECT_API IStaffSkillInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Staff_Q_Skill() = 0;
	virtual void Staff_W_Skill() = 0;
};
