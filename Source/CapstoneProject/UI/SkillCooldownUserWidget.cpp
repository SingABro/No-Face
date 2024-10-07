// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillCooldownUserWidget.h"
#include "Components/ProgressBar.h"

USkillCooldownUserWidget::USkillCooldownUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxCooldownTime = -1.f;
	bIsCooldownActive = false;
}

void USkillCooldownUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CooldownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("SkillCooldownBar")));
	ensure(CooldownBar);

	CooldownBar->SetPercent(0.f);
}

void USkillCooldownUserWidget::UpdateCooldownBar(float CurrentTime)
{
	if (bIsCooldownActive)
	{
		CooldownBar->SetPercent(CurrentTime / MaxCooldownTime);

		if (CurrentTime >= MaxCooldownTime - KINDA_SMALL_NUMBER)
		{
			bIsCooldownActive = false;		
			CooldownBar->SetPercent(0.f);
		}
	}
}

void USkillCooldownUserWidget::SetCooldownTime(float InMaxCooldownTime)
{
	MaxCooldownTime = InMaxCooldownTime;
}

void USkillCooldownUserWidget::StartCooldown()
{
	bIsCooldownActive = true;
}
