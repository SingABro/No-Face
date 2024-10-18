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

	

	
}

void ACPlayerController::BeginPlay()
{
	HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}

	
	
	

	///* 해당 옵션을 설정해야지 PlayerController에서 인풋시스템을 관리하는듯 하다 -> 아닌가? */
	//FInputModeGameOnly GameOnlyInputMode;
	//SetInputMode(GameOnlyInputMode);
}

