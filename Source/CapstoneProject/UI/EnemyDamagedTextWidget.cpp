// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyDamagedTextWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

UEnemyDamagedTextWidget::UEnemyDamagedTextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DamagedTextEndDelegate.BindDynamic(this, &UEnemyDamagedTextWidget::AnimationFinished);

}

void UEnemyDamagedTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("DamagedText")));
	ensure(TextBlock);

	BindToAnimationFinished(FadeOut, DamagedTextEndDelegate);
}

void UEnemyDamagedTextWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	FVector2D CurrentPosition = RenderTransform.Translation;
	CurrentPosition.Y -= 30 * DeltaTime; // Y 방향으로 천천히 위로 이동
	SetRenderTranslation(CurrentPosition);
}

void UEnemyDamagedTextWidget::SetDamagedText(const FString& Text)
{
	if (TextBlock)
	{
		TextBlock->SetText(FText::FromString(Text));

		// 애니메이션 실행
		if (FadeOut)
		{
			PlayAnimation(FadeOut);
		}
	}
}

void UEnemyDamagedTextWidget::AnimationFinished()
{
	RemoveFromViewport();
}
