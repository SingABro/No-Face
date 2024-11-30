// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API USkillUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USkillUIWidget(const FObjectInitializer& OnjectInitializer);

protected:
	virtual void NativeConstruct() override;

};
