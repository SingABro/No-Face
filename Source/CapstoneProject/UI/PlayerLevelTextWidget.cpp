// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerLevelTextWidget.h"
#include "Components/TextBlock.h"

UPlayerLevelTextWidget::UPlayerLevelTextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CurrentLevel = 1;
}

void UPlayerLevelTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerLevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelText")));
	ensure(PlayerLevelText);
}

void UPlayerLevelTextWidget::UpdateLevelText(int NewLevel)
{
	FText level = FText::FromString(FString::FromInt(NewLevel));
}
