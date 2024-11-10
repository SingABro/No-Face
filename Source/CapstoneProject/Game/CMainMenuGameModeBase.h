// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CMainMenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API ACMainMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACMainMenuGameModeBase();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UMainMenuWidget> MainMenuWidget;
};
