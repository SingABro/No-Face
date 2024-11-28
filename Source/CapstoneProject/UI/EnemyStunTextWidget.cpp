// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyStunTextWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "TimerManager.h"


UEnemyStunTextWidget::UEnemyStunTextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	StunTextEndDelegate.BindDynamic(this, &UEnemyStunTextWidget::AnimationFinished);


}

void UEnemyStunTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("StunText")));
	ensure(TextBlock);

	FSlateFontInfo CurrentFont = TextBlock->Font;
	CurrentFont.Size = 35;
	TextBlock->SetFont(CurrentFont);

	FVector2D CurrentPosition = RenderTransform.Translation;
	float dx = FMath::FRandRange(-25.0f, 25.0f);
	float dy = FMath::FRandRange(-25.0f, 25.0f);
	CurrentPosition.X += dx;
	CurrentPosition.Y += dy;
	SetRenderTranslation(CurrentPosition);

	BindToAnimationFinished(FadeOut, StunTextEndDelegate);
}

void UEnemyStunTextWidget::SetStunText()
{
	if (TextBlock)
	{
		FTimerHandle FontTimer;
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(FontTimer, this, &UEnemyStunTextWidget::ChangeFontSize, 0.1f, false);
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &UEnemyStunTextWidget::DelayedAction, 0.75f, false);
	}
}

void UEnemyStunTextWidget::ChangeFontSize()
{
	FSlateFontInfo CurrentFont = TextBlock->Font;
	while (CurrentFont.Size > 30) {
		CurrentFont.Size -= 2.0f;
		TextBlock->SetFont(CurrentFont);
	}
}

void UEnemyStunTextWidget::DelayedAction()
{
	// 애니메이션 실행
	if (FadeOut)
	{
		PlayAnimation(FadeOut, 0.0f, 1, EUMGSequencePlayMode::Forward, 15.0f);
	}
}

void UEnemyStunTextWidget::AnimationFinished()
{
	RemoveFromViewport();
}
