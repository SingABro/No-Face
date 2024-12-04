// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldmapWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UWorldmapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWorldmapWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void FindActorsByNameContains();
	void ChangeImageColor(int index);

	void FourSideChange(int index);
	void UpSideChange(int index);
	void DownSideChange(int index);
	void LeftSideChange(int index);
	void RightSideChange(int index);
	

private:
	UPROPERTY()
	TArray<TObjectPtr<class UWorldmapImageWidget>> ImageWidgets;

	int temp_identity[225];
	int temp_state[225];

	int bindex;
};


