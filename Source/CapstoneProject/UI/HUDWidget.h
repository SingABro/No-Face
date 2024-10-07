// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/PlayerHUDInterface.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHUDWidget(const FObjectInitializer& OnjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateHpBar(float NewCurrentHp);
	void UpdateExpBar(float NewCurrentExp);
	void SetMaxHp(float NewMaxHp);

	void SetMaxCooldown(float InMaxCooldownTime, ESkillType SkillType);
	void StartCooldown(ESkillType SkillType);
	void UpdateCooldownBar(float CurrentTime, ESkillType SkillType);


protected:
	UPROPERTY()
	TObjectPtr<class UPlayerHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UPlayerExpBarWidget> ExpBar;

	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> SkillCooldownBar_Q;
	TObjectPtr<class USkillCooldownUserWidget> SkillCooldownBar_W;
	TObjectPtr<class USkillCooldownUserWidget> SkillCooldownBar_E;
	TObjectPtr<class USkillCooldownUserWidget> SkillCooldownBar_R;

};
