// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Controller/AIControllerBase.h"
#include "AIControllerHelix.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API AAIControllerHelix : public AAIControllerBase
{
	GENERATED_BODY()
	
public:
	AAIControllerHelix();

public:
	class ACharacter* GetTarget();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
};
