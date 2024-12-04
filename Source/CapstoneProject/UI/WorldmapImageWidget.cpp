// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WorldmapImageWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

UWorldmapImageWidget::UWorldmapImageWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWorldmapImageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WorldmapImage = Cast<UImage>(GetWidgetFromName(TEXT("WorldmapImage")));
	ensure(WorldmapImage);

    FVector2D ImgSize(50.0f, 50.0f);
    WorldmapImage->SetBrushSize(ImgSize);

    
}

void UWorldmapImageWidget::SetThisImage(UTexture2D* NewTexture)
{
    if (NewTexture)
    {
        WorldmapImage->SetBrushFromTexture(NewTexture);
    }
}

void UWorldmapImageWidget::SetRoomStatus(FLinearColor newColor)
{
    WorldmapImage->SetColorAndOpacity(newColor);
}

