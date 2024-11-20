// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StoryBookWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnLastPage)
/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UStoryBookWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UStoryBookWidget(const FObjectInitializer& ObjectInitializer);

	FOnLastPage OnLastPage;

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void NextPage();

private:
	UPROPERTY()
	TObjectPtr<class UButton> NextPageButton;

	UPROPERTY()
	TArray<class UTextBlock*> Texts;

	int32 MaxIndex = 5;
	int32 Index = 0;
};
