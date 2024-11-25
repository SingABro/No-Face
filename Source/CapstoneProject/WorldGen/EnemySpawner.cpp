// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGen/EnemySpawner.h"
#include "Enemy/EnemyMelee_Common.h"
#include "Enemy/EnemyMelee_Assassin.h"
#include "Enemy/EnemyMelee_Tanker.h"
#include "Enemy/EnemyRanged_Rifle.h"
#include "Enemy/EnemyRanged_Siege.h"
#include "Enemy/EnemyBoss_Helix.h"
#include "NavigationSystem.h"

UEnemySpawner::UEnemySpawner()
{
	static ConstructorHelpers::FClassFinder<AEnemyMelee_Common> CommonClassRef(TEXT("/Game/No-Face/Blueprints/Enemy/BP_EnemyMelee_Common.BP_EnemyMelee_Common_C"));
	if (CommonClassRef.Class)
	{
		Enemys.Add(CommonClassRef.Class);
	}
	static ConstructorHelpers::FClassFinder<AEnemyMelee_Assassin> AssassinClassRef(TEXT("/Game/No-Face/Blueprints/Enemy/BP_EnemyMelee_Assassin.BP_EnemyMelee_Assassin_C"));
	if (AssassinClassRef.Class)
	{
		Enemys.Add(AssassinClassRef.Class);
	}
	static ConstructorHelpers::FClassFinder<AEnemyMelee_Tanker> TankerClassRef(TEXT("/Game/No-Face/Blueprints/Enemy/BP_EnemyMelee_Tanker.BP_EnemyMelee_Tanker_C"));
	if (TankerClassRef.Class)
	{
		Enemys.Add(TankerClassRef.Class);
	}
	static ConstructorHelpers::FClassFinder<AEnemyRanged_Rifle> RifleClassRef(TEXT("/Game/No-Face/Blueprints/Enemy/BP_EnemyRanged_Rifle.BP_EnemyRanged_Rifle_C"));
	if (RifleClassRef.Class)
	{
		Enemys.Add(RifleClassRef.Class);
	}
	static ConstructorHelpers::FClassFinder<AEnemyRanged_Siege> SiegeClassRef(TEXT("/Game/No-Face/Blueprints/Enemy/BP_EnemyRanged_Siege.BP_EnemyRanged_Siege_C"));
	if (SiegeClassRef.Class)
	{
		Enemys.Add(SiegeClassRef.Class);
	}
	static ConstructorHelpers::FClassFinder<AEnemyBoss_Helix> HelixClassRef(TEXT("/Game/No-Face/Blueprints/Enemy/BP_EnemyBoss_Helix.BP_EnemyBoss_Helix_C"));
	if (HelixClassRef.Class)
	{
		Enemys.Add(HelixClassRef.Class);
	}
}

void UEnemySpawner::GenerateEnemy(bool bIsBoss, int32 Depth, const FVector& SpawnLocation)
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation SpawnRandomPos;

	if (bIsBoss == false)
	{
		TSet<FVector> RandomPos;
		for (int i = 0; i < Depth + 1; ++i)
		{
			if (NavSystem->GetRandomPointInNavigableRadius(SpawnLocation, 2500, SpawnRandomPos))
			{
				SpawnRandomPos.Location.Z = 90.f;
				RandomPos.Add(SpawnRandomPos);
			}
		}
		for (const auto& SpawnPos : RandomPos)
		{
			int32 Index = FMath::RandRange(0, 4);
			GetWorld()->SpawnActor<AEnemyBase>(Enemys[Index], SpawnPos, FRotator::ZeroRotator);
		}

	}
	else
	{
		GetWorld()->SpawnActor<AEnemyBase>(Enemys[5], SpawnLocation + FVector(0.f, 0.f, 600.f), FRotator::ZeroRotator);
	}
	
}
