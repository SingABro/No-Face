// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"
#include "PlayerHpBarWidget.h"
#include "PlayerExpBarWidget.h"
#include "UI/SkillCooldownUserWidget.h"
#include "Character/CharacterBase.h"

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

	Sword_SkillCooldownBar_Q = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_Q")));
	ensure(Sword_SkillCooldownBar_Q);
	Bow_SkillCooldownBar_Q = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_Q_1")));
	ensure(Bow_SkillCooldownBar_Q);
	Staff_SkillCooldownBar_Q = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_Q_2")));
	ensure(Staff_SkillCooldownBar_Q);

	Sword_SkillCooldownBar_W = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_W")));
	ensure(Sword_SkillCooldownBar_W);
	Bow_SkillCooldownBar_W = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_W_1")));
	ensure(Bow_SkillCooldownBar_W);
	Staff_SkillCooldownBar_W = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_W_2")));
	ensure(Staff_SkillCooldownBar_W);
	
	Sword_SkillCooldownBar_E = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_E")));
	ensure(Sword_SkillCooldownBar_E);
	Bow_SkillCooldownBar_E = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_E_1")));
	ensure(Bow_SkillCooldownBar_E);
	Staff_SkillCooldownBar_E = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_E_2")));
	ensure(Staff_SkillCooldownBar_E);
	
	Sword_SkillCooldownBar_R = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_R")));
	ensure(Sword_SkillCooldownBar_R);
	Bow_SkillCooldownBar_R = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_R_1")));
	ensure(Bow_SkillCooldownBar_R);
	Staff_SkillCooldownBar_R = Cast<USkillCooldownUserWidget>(GetWidgetFromName(TEXT("WidgetCooldownBar_R_2")));
	ensure(Staff_SkillCooldownBar_R);

	ACharacterBase* Character = Cast<ACharacterBase>(GetOwningPlayerPawn());
	if (Character)
	{
		Character->SignedChangeWeapon.AddUObject(this, &UHUDWidget::SetSkillUI);
		Character->SetupHUDWidget(this);
	}

	SetSwordSkillUI();
}

void UHUDWidget::SetMaxHp(float NewMaxHp)
{
	HpBar->SetMaxHp(NewMaxHp);
}

void UHUDWidget::SetMaxCooldown(float InMaxCooldownTime, int32 WeaponType, ESkillType SkillType)
{
	switch (WeaponType)
	{
	case 0:
		switch (SkillType)
		{
		case ESkillType::Q:
			Sword_SkillCooldownBar_Q->SetCooldownTime(InMaxCooldownTime);
			break;
		case ESkillType::W:
			Sword_SkillCooldownBar_W->SetCooldownTime(InMaxCooldownTime);
			break;
		case ESkillType::E:
			Sword_SkillCooldownBar_E->SetCooldownTime(InMaxCooldownTime);
			break;
		case ESkillType::R:
			Sword_SkillCooldownBar_R->SetCooldownTime(InMaxCooldownTime);
			break;
		default:
			break;
		}
		break;
	case 1:
		switch (SkillType)
		{
		case ESkillType::Q:
			Bow_SkillCooldownBar_Q->SetCooldownTime(InMaxCooldownTime);
			break;
		case ESkillType::W:
			Bow_SkillCooldownBar_W->SetCooldownTime(InMaxCooldownTime);
			break;
		case ESkillType::E:
			Bow_SkillCooldownBar_E->SetCooldownTime(InMaxCooldownTime);
			break;
		case ESkillType::R:
			Bow_SkillCooldownBar_R->SetCooldownTime(InMaxCooldownTime);
			break;
		default:
			break;
		}
		break;
	case 2:
		switch (SkillType)
		{
		case ESkillType::Q:
			Staff_SkillCooldownBar_Q->SetCooldownTime(InMaxCooldownTime);
			break;
		case ESkillType::W:
			Staff_SkillCooldownBar_W->SetCooldownTime(InMaxCooldownTime);
			break;
		case ESkillType::E:
			Staff_SkillCooldownBar_E->SetCooldownTime(InMaxCooldownTime);
			break;
		case ESkillType::R:
			Staff_SkillCooldownBar_R->SetCooldownTime(InMaxCooldownTime);
			break;
		default:
			break;
		}
		break;
	}
}

void UHUDWidget::StartCooldown(int32 WeaponType, ESkillType SkillType)
{
	switch (WeaponType)
	{
	case 0:
		switch (SkillType)
		{
		case ESkillType::Q:
			Sword_SkillCooldownBar_Q->StartCooldown();
			break;
		case ESkillType::W:
			Sword_SkillCooldownBar_W->StartCooldown();
			break;
		case ESkillType::E:
			Sword_SkillCooldownBar_E->StartCooldown();
			break;
		case ESkillType::R:
			Sword_SkillCooldownBar_R->StartCooldown();
			break;
		default:
			break;
		}
		break;
	case 1:
		switch (SkillType)
		{
		case ESkillType::Q:
			Bow_SkillCooldownBar_Q->StartCooldown();
			break;
		case ESkillType::W:
			Bow_SkillCooldownBar_W->StartCooldown();
			break;
		case ESkillType::E:
			Bow_SkillCooldownBar_E->StartCooldown();
			break;
		case ESkillType::R:
			Bow_SkillCooldownBar_R->StartCooldown();
			break;
		default:
			break;
		}
		break;
	case 2:
		switch (SkillType)
		{
		case ESkillType::Q:
			Staff_SkillCooldownBar_Q->StartCooldown();
			break;
		case ESkillType::W:
			Staff_SkillCooldownBar_W->StartCooldown();
			break;
		case ESkillType::E:
			Staff_SkillCooldownBar_E->StartCooldown();
			break;
		case ESkillType::R:
			Staff_SkillCooldownBar_R->StartCooldown();
			break;
		default:
			break;
		}
		break;
	}
}

void UHUDWidget::UpdateCooldownBar(float CooldownDuration, FTimerHandle& CooldownTimerHandle, bool& bCanUseSkill, ESkillType SkillType, int32 WeaponType, float& Timer)
{
	switch (WeaponType)
	{
	case 0:
		switch (SkillType)
		{
		case ESkillType::Q:
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
				[&, SkillType, CooldownDuration]()
				{
					float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
					Timer += ElapsedTime;

					Sword_SkillCooldownBar_Q->UpdateCooldownBar(Timer);

					if (Timer >= CooldownDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
						bCanUseSkill = true;
						Timer = 0.f;
					}
				}, 0.01f, true);
			break;
		case ESkillType::W:
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
				[&, SkillType, CooldownDuration]() 
				{
					float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
					Timer += ElapsedTime;

					Sword_SkillCooldownBar_W->UpdateCooldownBar(Timer);

					if (Timer >= CooldownDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
						bCanUseSkill = true;
						Timer = 0.f;
					}
				}, 0.01f, true);
			break;
		case ESkillType::E:
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
				[&, SkillType, CooldownDuration]() 
				{
					float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
					Timer += ElapsedTime;

					Sword_SkillCooldownBar_E->UpdateCooldownBar(Timer);

					if (Timer >= CooldownDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
						bCanUseSkill = true;
						Timer = 0.f;
					}
				}, 0.01f, true);
			break;
		case ESkillType::R:
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
				[&, SkillType, CooldownDuration]()
				{
					float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
					Timer += ElapsedTime;

					Sword_SkillCooldownBar_R->UpdateCooldownBar(Timer);

					if (Timer >= CooldownDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
						bCanUseSkill = true;
						Timer = 0.f;
					}
				}, 0.01f, true);
			break;
		default:
			break;
		}
		break;
	case 1:
		switch (SkillType)
		{
		case ESkillType::Q:
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
				[&, SkillType, CooldownDuration]()
				{
					float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
					Timer += ElapsedTime;

					Bow_SkillCooldownBar_Q->UpdateCooldownBar(Timer);

					if (Timer >= CooldownDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
						bCanUseSkill = true;
						Timer = 0.f;
					}
				}, 0.01f, true);
			break;
		case ESkillType::W:
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
				[&, SkillType, CooldownDuration]() 
				{
					float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
					Timer += ElapsedTime;

					Bow_SkillCooldownBar_W->UpdateCooldownBar(Timer);

					if (Timer >= CooldownDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
						bCanUseSkill = true;
						Timer = 0.f;
					}
				}, 0.01f, true);
			break;
		case ESkillType::E:
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
				[&, SkillType, CooldownDuration]()
				{
					float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
					Timer += ElapsedTime;

					Bow_SkillCooldownBar_E->UpdateCooldownBar(Timer);

					if (Timer >= CooldownDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
						bCanUseSkill = true;
						Timer = 0.f;
					}
				}, 0.01f, true);
			break;
		case ESkillType::R:
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
				[&, SkillType, CooldownDuration]()
				{
					float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
					Timer += ElapsedTime;

					Bow_SkillCooldownBar_R->UpdateCooldownBar(Timer);

					if (Timer >= CooldownDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
						bCanUseSkill = true;
						Timer = 0.f;
					}
				}, 0.01f, true);
			break;
		default:
			break;
		}
		break;
	case 2:
		switch (SkillType)
		{
		case ESkillType::Q:
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
				[&, SkillType, CooldownDuration]()
				{
					float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
					Timer += ElapsedTime;

					Staff_SkillCooldownBar_Q->UpdateCooldownBar(Timer);

					if (Timer >= CooldownDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
						bCanUseSkill = true;
						Timer = 0.f;
					}
				}, 0.01f, true);
			break;
		case ESkillType::W:
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
				[&, SkillType, CooldownDuration]()
				{
					float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
					Timer += ElapsedTime;

					Staff_SkillCooldownBar_W->UpdateCooldownBar(Timer);

					if (Timer >= CooldownDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
						bCanUseSkill = true;
						Timer = 0.f;
					}
				}, 0.01f, true);
			break;
		case ESkillType::E:
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
				[&, SkillType, CooldownDuration]()
				{
					float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
					Timer += ElapsedTime;

					Staff_SkillCooldownBar_E->UpdateCooldownBar(Timer);

					if (Timer >= CooldownDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
						bCanUseSkill = true;
						Timer = 0.f;
					}
				}, 0.01f, true);
			break;
		case ESkillType::R:
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
				[&, SkillType, CooldownDuration]() 
				{
					float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
					Timer += ElapsedTime;

					Staff_SkillCooldownBar_R->UpdateCooldownBar(Timer);

					if (Timer >= CooldownDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
						bCanUseSkill = true;
						Timer = 0.f;
					}
				}, 0.01f, true);
			break;
		default:
			break;
		}
		break;
	}
}

void UHUDWidget::SetSkillUI(int32 WeaponType)
{
	switch (WeaponType)
	{
	case 0:
		SetSwordSkillUI();
		break;
	case 1:
		SetBowSkillUI();
		break;
	case 2:
		SetStaffSkillUI();
		break;
	default :
		break;
	}
}

void UHUDWidget::SetSwordSkillUI()
{
	Sword_SkillCooldownBar_Q->SetVisibility(ESlateVisibility::Visible);
	Sword_SkillCooldownBar_W->SetVisibility(ESlateVisibility::Visible);
	Sword_SkillCooldownBar_E->SetVisibility(ESlateVisibility::Visible);
	Sword_SkillCooldownBar_R->SetVisibility(ESlateVisibility::Visible);

	Bow_SkillCooldownBar_Q->SetVisibility(ESlateVisibility::Hidden);
	Bow_SkillCooldownBar_W->SetVisibility(ESlateVisibility::Hidden);
	Bow_SkillCooldownBar_E->SetVisibility(ESlateVisibility::Hidden);
	Bow_SkillCooldownBar_R->SetVisibility(ESlateVisibility::Hidden);

	Staff_SkillCooldownBar_Q->SetVisibility(ESlateVisibility::Hidden);
	Staff_SkillCooldownBar_W->SetVisibility(ESlateVisibility::Hidden);
	Staff_SkillCooldownBar_E->SetVisibility(ESlateVisibility::Hidden);
	Staff_SkillCooldownBar_R->SetVisibility(ESlateVisibility::Hidden);
}

void UHUDWidget::SetBowSkillUI()
{
	Sword_SkillCooldownBar_Q->SetVisibility(ESlateVisibility::Hidden);
	Sword_SkillCooldownBar_W->SetVisibility(ESlateVisibility::Hidden);
	Sword_SkillCooldownBar_E->SetVisibility(ESlateVisibility::Hidden);
	Sword_SkillCooldownBar_R->SetVisibility(ESlateVisibility::Hidden);

	Bow_SkillCooldownBar_Q->SetVisibility(ESlateVisibility::Visible);
	Bow_SkillCooldownBar_W->SetVisibility(ESlateVisibility::Visible);
	Bow_SkillCooldownBar_E->SetVisibility(ESlateVisibility::Visible);
	Bow_SkillCooldownBar_R->SetVisibility(ESlateVisibility::Visible);

	Staff_SkillCooldownBar_Q->SetVisibility(ESlateVisibility::Hidden);
	Staff_SkillCooldownBar_W->SetVisibility(ESlateVisibility::Hidden);
	Staff_SkillCooldownBar_E->SetVisibility(ESlateVisibility::Hidden);
	Staff_SkillCooldownBar_R->SetVisibility(ESlateVisibility::Hidden);
}

void UHUDWidget::SetStaffSkillUI()
{
	Sword_SkillCooldownBar_Q->SetVisibility(ESlateVisibility::Hidden);
	Sword_SkillCooldownBar_W->SetVisibility(ESlateVisibility::Hidden);
	Sword_SkillCooldownBar_E->SetVisibility(ESlateVisibility::Hidden);
	Sword_SkillCooldownBar_R->SetVisibility(ESlateVisibility::Hidden);

	Bow_SkillCooldownBar_Q->SetVisibility(ESlateVisibility::Hidden);
	Bow_SkillCooldownBar_W->SetVisibility(ESlateVisibility::Hidden);
	Bow_SkillCooldownBar_E->SetVisibility(ESlateVisibility::Hidden);
	Bow_SkillCooldownBar_R->SetVisibility(ESlateVisibility::Hidden);

	Staff_SkillCooldownBar_Q->SetVisibility(ESlateVisibility::Visible);
	Staff_SkillCooldownBar_W->SetVisibility(ESlateVisibility::Visible);
	Staff_SkillCooldownBar_E->SetVisibility(ESlateVisibility::Visible);
	Staff_SkillCooldownBar_R->SetVisibility(ESlateVisibility::Visible);
}

void UHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UHUDWidget::UpdateExpBar(float NewCurrentExp)
{
	ExpBar->UpdateExpBar(NewCurrentExp);
}

