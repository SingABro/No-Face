// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGen/EnemySpawnerComponent.h"
#include "Enemy/EnemyBase.h"

UEnemySpawnerComponent::UEnemySpawnerComponent()
{

}

void UEnemySpawnerComponent::GenerateEnemy(bool bIsBoss, int32 Depth, const FVector& SpawnLocation)
{
	int32 Index = FMath::RandRange(0, 4);
	GetWorld()->SpawnActor<AEnemyBase>(Enemys[Index], SpawnLocation, FRotator::ZeroRotator);
}



