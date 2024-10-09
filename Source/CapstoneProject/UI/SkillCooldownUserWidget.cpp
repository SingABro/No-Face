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

	//처음 UI 생성시 쿨타임바 초기화
	CooldownBar->SetPercent(0.f);
}

void USkillCooldownUserWidget::UpdateCooldownBar(float CurrentTime)
{
	//StartCooldown 함수가 명시적으로 호출되어야지 쿨타임바의 업데이트 시작
	if (bIsCooldownActive)
	{
		//외부에서 들어오는 시간을 그대로 적용한다.
		CooldownBar->SetPercent(CurrentTime / MaxCooldownTime);

		//최대 쿨타임보다 커지면 더 이상 업데이트를 진행하지 않으며 쿨타임바를 초기화한다.
		if (CurrentTime >= MaxCooldownTime - KINDA_SMALL_NUMBER)
		{
			bIsCooldownActive = false;		
			CooldownBar->SetPercent(0.f);
		}
	}
}

//최대 쿨타임 시간을 정하는 함수
void USkillCooldownUserWidget::SetCooldownTime(float InMaxCooldownTime)
{
	MaxCooldownTime = InMaxCooldownTime;
}

//쿨타임바의 실행 트리거 함수
void USkillCooldownUserWidget::StartCooldown()
{
	bIsCooldownActive = true;
}
