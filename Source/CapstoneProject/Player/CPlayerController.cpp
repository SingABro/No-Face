// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerController.h"
#include "UI/HUDWidget.h"

ACPlayerController::ACPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	static ConstructorHelpers::FClassFinder<UHUDWidget> HUDWidgetRef(TEXT("/Game/No-Face/UI/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetRef.Class)
	{
		HUDWidgetClass = HUDWidgetRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> DeadWidgetRef(TEXT("/Game/No-Face/UI/WBP_DeadUI.WBP_DeadUI_C"));
	if (DeadWidgetRef.Class)
	{
		DeadScreenClass = DeadWidgetRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> MapUIWidgetRef(TEXT("/Game/No-Face/UI/WBP_MapUI.WBP_MapUI_C"));
	if (MapUIWidgetRef.Class)
	{
		MapUIClass = MapUIWidgetRef.Class;
	}

}

void ACPlayerController::BeginPlay()
{
	HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
	UUserWidget* MapUIWidget = CreateWidget(this, MapUIClass);
	if (MapUIWidget)
	{
		MapUIWidget->AddToViewport();
	}

	///* 해당 옵션을 설정해야지 PlayerController에서 인풋시스템을 관리하는듯 하다 -> 아닌가? */
	//FInputModeGameOnly GameOnlyInputMode;
	//SetInputMode(GameOnlyInputMode);
}

void ACPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	UUserWidget* DeadScreen = CreateWidget(this, DeadScreenClass);
	if (DeadScreen)
	{
		DeadScreen->AddToViewport();
	}

	FTimerHandle ResponeTimer;
	GetWorldTimerManager().SetTimer(ResponeTimer, this, &ACPlayerController::RestartLevel, 5.0f);
}

