// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillCooldownUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API USkillCooldownUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USkillCooldownUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateCooldownBar(float CurrentTime);
	void SetCooldownTime(float InMaxCooldownTime);
	void StartCooldown();

private:
	UPROPERTY()
	TObjectPtr<class UProgressBar> CooldownBar;

	bool bIsCooldownActive;
	float MaxCooldownTime;
};
