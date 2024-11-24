// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnemySpawner.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UEnemySpawner : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UEnemySpawner();

	void GenerateEnemy(bool bIsBoss, int32 Depth, const FVector& SpawnLocation);

private:
	UPROPERTY(EditAnywhere, Category = "Enemy")
	TArray<TSubclassOf<class AEnemyBase>> Enemys;
};
