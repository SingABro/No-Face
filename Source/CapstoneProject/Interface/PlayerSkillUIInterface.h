// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerSkillUIInterface.generated.h"

UENUM()
enum class ESkillType : uint8
{
	Q = 0,
	W,
	E,
	R
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerSkillUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CAPSTONEPROJECT_API IPlayerSkillUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetupSkillUIWidget(class UHUDWidget* InHUDWidget) = 0;
};
