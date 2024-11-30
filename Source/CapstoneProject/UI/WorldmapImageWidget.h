// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldmapImageWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UWorldmapImageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWorldmapImageWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetThisImage(UTexture2D* NewTexture);
	void SetRoomStatus(FLinearColor newColor);

private:
	UPROPERTY()
	TObjectPtr<class UImage> WorldmapImage;
};