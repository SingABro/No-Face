// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemySpawnerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAPSTONEPROJECT_API UEnemySpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemySpawnerComponent();

public:
	void GenerateEnemy(bool bIsBoss, int32 Depth, const FVector& SpawnLocation);

private:
	UPROPERTY(EditAnywhere, Category = "Enemy")
	TArray<TSubclassOf<class AEnemyBase>> Enemys;
};
