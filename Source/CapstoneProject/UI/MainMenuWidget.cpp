// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("Start")));
	ensure(StartButton);
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("Quit")));
	ensure(ExitButton);

	StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonClicked);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);

}

void UMainMenuWidget::OnStartButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName("Test"));
}

void UMainMenuWidget::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

