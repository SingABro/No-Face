// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/EnemyPtrWidget.h"
#include "EnemyDamagedTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UEnemyDamagedTextWidget : public UEnemyPtrWidget
{
	GENERATED_BODY()

public:
	UEnemyDamagedTextWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetDamagedText(const FString& Text);
	void DelayedAction();
	void ChangeFontSize();

	UFUNCTION()
	void AnimationFinished();
	
	FWidgetAnimationDynamicEvent DamagedTextEndDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> FadeOut;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Text", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> TextBlock;
};
