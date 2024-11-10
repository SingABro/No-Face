// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CMainMenuGameModeBase.h"
#include "UI/MainMenuWidget.h"
#include "GameFramework/PlayerController.h"

ACMainMenuGameModeBase::ACMainMenuGameModeBase()
{
	static ConstructorHelpers::FClassFinder<UMainMenuWidget> MainMenuWidgetClassRef(TEXT("/Game/No-Face/UI/WBP_MainMenu.WBP_MainMenu_C"));
	if (MainMenuWidgetClassRef.Class)
	{
		MainMenuWidgetClass = MainMenuWidgetClassRef.Class;
	}
}

void ACMainMenuGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	if (MainMenuWidget)
	{
		MainMenuWidget->AddToViewport();
	}
}
