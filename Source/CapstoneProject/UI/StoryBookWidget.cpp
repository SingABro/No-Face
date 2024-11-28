// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StoryBookWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

UStoryBookWidget::UStoryBookWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UStoryBookWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NextPageButton = Cast<UButton>(GetWidgetFromName(TEXT("NextPage")));

	for (int i = 1; i <= MaxIndex; ++i)
	{
		FName TextName = *FString::Printf(TEXT("Text%d"), i);
		Texts.Add(Cast<UTextBlock>(GetWidgetFromName(TextName)));
	}

	Texts[0]->SetVisibility(ESlateVisibility::Visible);
	for (int i = 1; i < MaxIndex; ++i)
	{
		Texts[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	NextPageButton->OnClicked.Clear();
	NextPageButton->OnClicked.AddDynamic(this, &UStoryBookWidget::NextPage);
}

void UStoryBookWidget::NextPage()
{
	Index++;
	if (MaxIndex <= Index)
	{
		OnLastPage.Broadcast();
		Index = 0;
		return;
	}

	Texts[Index - 1]->SetVisibility(ESlateVisibility::Collapsed);
	Texts[Index]->SetVisibility(ESlateVisibility::Visible);
}
