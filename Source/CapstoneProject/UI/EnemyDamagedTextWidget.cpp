// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyDamagedTextWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "TimerManager.h"


UEnemyDamagedTextWidget::UEnemyDamagedTextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DamagedTextEndDelegate.BindDynamic(this, &UEnemyDamagedTextWidget::AnimationFinished);


}

void UEnemyDamagedTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("DamagedText")));
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

	BindToAnimationFinished(FadeOut, DamagedTextEndDelegate);
}

void UEnemyDamagedTextWidget::SetDamagedText(const FString& Text)
{
	if (TextBlock)
	{
		TextBlock->SetText(FText::FromString(Text));

		FTimerHandle FontTimer;
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(FontTimer, this, &UEnemyDamagedTextWidget::ChangeFontSize, 0.1f, false);
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &UEnemyDamagedTextWidget::DelayedAction, 0.75f, false);

	}
}

void UEnemyDamagedTextWidget::ChangeFontSize()
{
	FSlateFontInfo CurrentFont = TextBlock->Font;
	while (CurrentFont.Size > 30) {
		CurrentFont.Size -= 2.0f;
		TextBlock->SetFont(CurrentFont);
	}
}

void UEnemyDamagedTextWidget::DelayedAction()
{
	// 애니메이션 실행
	if (FadeOut)
	{
		PlayAnimation(FadeOut, 0.0f, 1, EUMGSequencePlayMode::Forward, 15.0f);
	}
}

void UEnemyDamagedTextWidget::AnimationFinished()
{
	RemoveFromViewport();
}
