// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameModeBase();
	
	void GameEnd();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> WinScreenClass;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TObjectPtr<class UAudioComponent> AudioComponent;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TObjectPtr<class USoundWave> BGM;
};
