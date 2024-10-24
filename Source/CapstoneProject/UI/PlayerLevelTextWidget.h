// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerLevelTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UPlayerLevelTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPlayerLevelTextWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateLevelText(int NewLevel);

protected:
	UPROPERTY(VisibleAnywhere, Category = "LevelText")
	TObjectPtr<class UTextBlock> PlayerLevelText;

	UPROPERTY()
	float CurrentLevel;
};
