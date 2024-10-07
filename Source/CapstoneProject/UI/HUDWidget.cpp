// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"
#include "PlayerHpBarWidget.h"
#include "PlayerExpBarWidget.h"
#include "UI/SkillCooldownUserWidget.h"

UHUDWidget::UHUDWidget(const FObjectInitializer& OnjectInitializer) : Super(OnjectInitializer)
{

}

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UPlayerHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	ExpBar = Cast<UPlayerExpBarWidget>(GetWidgetFromName(TEXT("WidgetExpBar")));
	ensure(ExpBar);

	SkillCooldownBar_Q = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_Q")));
	ensure(SkillCooldownBar_Q);

	SkillCooldownBar_W = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_W")));
	ensure(SkillCooldownBar_W);
	
	SkillCooldownBar_E = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_E")));
	ensure(SkillCooldownBar_E);
	
	SkillCooldownBar_R = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_R")));
	ensure(SkillCooldownBar_R);

	IPlayerHUDInterface* HUDPawn = Cast< IPlayerHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}

}

void UHUDWidget::SetMaxHp(float NewMaxHp)
{
	HpBar->SetMaxHp(NewMaxHp);
}

void UHUDWidget::SetMaxCooldown(float InMaxCooldownTime, ESkillType SkillType)
{
	switch (SkillType)
	{
	case ESkillType::Q:
		SkillCooldownBar_Q->SetCooldownTime(InMaxCooldownTime);
		break;
	case ESkillType::W:
		SkillCooldownBar_W->SetCooldownTime(InMaxCooldownTime);
		break;
	case ESkillType::E:
		SkillCooldownBar_E->SetCooldownTime(InMaxCooldownTime);
		break;
	case ESkillType::R:
		SkillCooldownBar_R->SetCooldownTime(InMaxCooldownTime);
		break;
	default:
		break;
	}
}

void UHUDWidget::StartCooldown(ESkillType SkillType)
{
	switch (SkillType)
	{
	case ESkillType::Q:
		SkillCooldownBar_Q->StartCooldown();
		break;
	case ESkillType::W:
		SkillCooldownBar_W->StartCooldown();
		break;
	case ESkillType::E:
		SkillCooldownBar_E->StartCooldown();
		break;
	case ESkillType::R:
		SkillCooldownBar_R->StartCooldown();
		break;
	default:
		break;
	}
}

void UHUDWidget::UpdateCooldownBar(float CurrentTime, ESkillType SkillType)
{
	switch (SkillType)
	{
	case ESkillType::Q:
		SkillCooldownBar_Q->UpdateCooldownBar(CurrentTime);
		break;
	case ESkillType::W:
		SkillCooldownBar_W->UpdateCooldownBar(CurrentTime);
		break;
	case ESkillType::E:
		SkillCooldownBar_E->UpdateCooldownBar(CurrentTime);
		break;
	case ESkillType::R:
		SkillCooldownBar_R->UpdateCooldownBar(CurrentTime);
		break;
	default:
		break;
	}
}

void UHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UHUDWidget::UpdateExpBar(float NewCurrentExp)
{
	ExpBar->UpdateExpBar(NewCurrentExp);
}



