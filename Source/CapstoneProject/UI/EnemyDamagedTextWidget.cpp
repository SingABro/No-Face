// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyDamagedTextWidget.h"
#include "Components/TextBlock.h"

UEnemyDamagedTextWidget::UEnemyDamagedTextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEnemyDamagedTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("DamagedText")));
	ensure(TextBlock);

}

void UEnemyDamagedTextWidget::SetDamagedText(const FString& Text)
{
	if (TextBlock)
	{
		TextBlock->SetText(FText::FromString(Text));
	}
}
