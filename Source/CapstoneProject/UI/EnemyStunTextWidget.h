// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/EnemyPtrWidget.h"
#include "EnemyStunTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UEnemyStunTextWidget : public UEnemyPtrWidget
{
	GENERATED_BODY()

public:
	UEnemyStunTextWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetStunText();
	void DelayedAction();
	void ChangeFontSize();

	UFUNCTION()
	void AnimationFinished();

	FWidgetAnimationDynamicEvent StunTextEndDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> FadeOut;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Text", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> TextBlock;
};