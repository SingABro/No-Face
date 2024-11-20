// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/PlayerSkillUIInterface.h"
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
	void UpdateExpBar(float NewCurrentExp);

	void SetMaxCooldown(float InMaxCooldownTime, int32 WeaponType, ESkillType SkillType);
	void StartCooldown(int32 WeaponType, ESkillType SkillType);
	void UpdateCooldownBar(float CooldownDuration, FTimerHandle& CooldownTimerHandle, bool& bCanUseSkill, ESkillType SkillType, int32 WeaponType, float& Timer);


private:
	void SetSkillUI(int32 WeaponType);
	void SetSwordSkillUI();
	void SetBowSkillUI();
	void SetStaffSkillUI();

private:

	UPROPERTY()
	TObjectPtr<class UPlayerExpBarWidget> ExpBar;

	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> Sword_SkillCooldownBar_Q;

	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> Bow_SkillCooldownBar_Q;

	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> Staff_SkillCooldownBar_Q;

	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> Sword_SkillCooldownBar_W;

	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> Bow_SkillCooldownBar_W;

	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> Staff_SkillCooldownBar_W;
	
	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> Sword_SkillCooldownBar_E;

	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> Bow_SkillCooldownBar_E;

	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> Staff_SkillCooldownBar_E;
	
	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> Sword_SkillCooldownBar_R;

	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> Bow_SkillCooldownBar_R;

	UPROPERTY()
	TObjectPtr<class USkillCooldownUserWidget> Staff_SkillCooldownBar_R;


};
