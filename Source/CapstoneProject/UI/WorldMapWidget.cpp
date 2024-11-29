// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WorldMapWidget.h"
#include "UI/WorldmapImageWidget.h"
#include "Components/UniformGridPanel.h"
#include "Engine/Texture2D.h"
#include "WorldGen/RoomActor.h"
#include "Components/SizeBox.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"

UWorldmapWidget::UWorldmapWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWorldmapWidget::NativeConstruct()
{
	Super::NativeConstruct();

    for (int32 i = 0; i < 225; i++) {
        temp_identity[i] = -1;
        FString WidgetName = FString::Printf(TEXT("WBP_WorldMapImage_%d"), i);
        UWorldmapImageWidget* ImageWidget = Cast<UWorldmapImageWidget>(GetWidgetFromName(*WidgetName));

        if (ImageWidget)
        {
            ImageWidgets.Add(ImageWidget);  // TArray에 위젯 추가
        }
        FString TexturePath = TEXT("Texture2D'/Game/No-Face/UI/Image/NoneMap.NoneMap'");
        UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);
        ImageWidgets[i]->SetRoomStatus(FLinearColor(1.0f, 1.0f, 1.0f, 0.01f));

        if (LoadedTexture)
        {
            ImageWidget->SetThisImage(LoadedTexture);
        }
    }
    
    FindActorsByNameContains();
    temp_identity[112] = 15;
    temp_state[112] = 2;
    
    ChangeImageColor(112);
    FourSideChange(112);
    ChangeImageColor(bindex);

    UE_LOG(LogTemp, Display, TEXT("Image Change Success."));


}

void UWorldmapWidget::FindActorsByNameContains(){

    TArray<ARoomActor*> FoundActors;
    // 월드 내 모든 액터 순회
    for (TActorIterator<ARoomActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        if (ActorItr->GetName().Contains("BP_RoomActor"))
        {
            FoundActors.Add(*ActorItr);  // 이름에 특정 문자열이 포함된 액터 추가
        }
    }

    int tx, ty, index;
    for (int i = 0; i < FoundActors.Num(); i++) {
        FVector tmp_location = FoundActors[i]->GetLocation();
        if (tmp_location.Z <= 1) {

            tx = -1 * tmp_location.X / 10000 + 7;
            ty = tmp_location.Y / 10000 + 7;
            index = tx * 15 + ty;

            temp_identity[index] = FoundActors[i]->GetIdentity();
            if (FoundActors[i]->GetStageState() == 0) {
                temp_state[index] = 0;
            }
            else if (FoundActors[i]->GetStageState() == 1) {
                temp_state[index] = 1;
                ChangeImageColor(index);
                FourSideChange(index);
            }
            else if (FoundActors[i]->GetStageState() == 2) {
                temp_state[index] = 2;
                ChangeImageColor(index);
            }

            if (FoundActors[i]->GetBossRoom()) {
                bindex = tx * 15 + ty;
            }


            FoundActors[i]->OnRoomActorEvent.AddUObject(this, &UWorldmapWidget::FindActorsByNameContains);
        }
    }
}

void UWorldmapWidget::ChangeImageColor(int index)
{
    FString TexturePath = FString::Printf(TEXT("Texture2D'/Game/No-Face/UI/Image/WM_Not%d.WM_Not%d'"), temp_identity[index], temp_identity[index]);
    UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);
    ImageWidgets[index]->SetThisImage(LoadedTexture);
    if (temp_state[index] == 0)
        ImageWidgets[index]->SetRoomStatus(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    else if (temp_state[index] > 0)
    {
        TexturePath = FString::Printf(TEXT("Texture2D'/Game/No-Face/UI/Image/WM_Clear%d.WM_Clear%d'"), temp_identity[index], temp_identity[index]);
        LoadedTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);
        ImageWidgets[index]->SetThisImage(LoadedTexture);
        if (temp_state[index] == 1)
            ImageWidgets[index]->SetRoomStatus(FLinearColor(0.4f, 0.8f, 0.4f, 1.0f));
        else if (temp_state[index] == 2)
            ImageWidgets[index]->SetRoomStatus(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }
    if (index == bindex) {
        TexturePath = FString::Printf(TEXT("Texture2D'/Game/No-Face/UI/Image/WM_Clear%d.WM_Clear%d'"), temp_identity[index], temp_identity[index]);
        LoadedTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);
        ImageWidgets[index]->SetThisImage(LoadedTexture);
        ImageWidgets[bindex]->SetRoomStatus(FLinearColor(0.72f, 0.0f, 0.0f, 1.0f));
    }
}

void UWorldmapWidget::FourSideChange(int index)
{
    switch (temp_identity[index])
    {
    case 1:
        LeftSideChange(index);
        break;
    case 2:
        UpSideChange(index);
        break;
    case 3:
        UpSideChange(index);
        LeftSideChange(index);
        break;
    case 4:
        RightSideChange(index);
        break;
    case 5:
        RightSideChange(index);
        LeftSideChange(index);
        break;
    case 6:
        UpSideChange(index);
        RightSideChange(index);
        break;
    case 7:
        UpSideChange(index);
        RightSideChange(index);
        LeftSideChange(index);
        break;
    case 8:
        DownSideChange(index);
        break;
    case 9:
        DownSideChange(index);
        LeftSideChange(index);
        break;
    case 10:
        UpSideChange(index);
        DownSideChange(index);
        break;
    case 11:
        UpSideChange(index);
        DownSideChange(index);
        LeftSideChange(index);
        break;
    case 12:
        DownSideChange(index);
        RightSideChange(index);
        break;
    case 13:
        DownSideChange(index);
        RightSideChange(index);
        LeftSideChange(index);
        break;
    case 14:
        UpSideChange(index);
        DownSideChange(index);
        RightSideChange(index);
        break;
    case 15:
        UpSideChange(index);
        DownSideChange(index);
        RightSideChange(index);
        LeftSideChange(index);
        break;
    }

}

void UWorldmapWidget::UpSideChange(int index)
{
    if (index / 15 != 0)
    {
        if (temp_identity[index - 15] != -1)
            ChangeImageColor(index - 15);
    }
}

void UWorldmapWidget::DownSideChange(int index)
{
    if (index / 15 != 14)
    {
        if (temp_identity[index + 15] != -1)
            ChangeImageColor(index + 15);
    }
}

void UWorldmapWidget::LeftSideChange(int index)
{
    if (index % 15 != 0) {
        if (temp_identity[index - 1] != -1)
            ChangeImageColor(index - 1);
    }
}

void UWorldmapWidget::RightSideChange(int index)
{
    if (index % 15 != 14)
    {
        if (temp_identity[index + 1] != -1)
            ChangeImageColor(index + 1);
    }
}