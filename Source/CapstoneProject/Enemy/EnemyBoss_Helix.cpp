// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBoss_Helix.h"
#include "Components/CapsuleComponent.h"
#include "Stat/EnemyStatComponent.h"

AEnemyBoss_Helix::AEnemyBoss_Helix()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Prime_Helix/Meshes/Prime_Helix.Prime_Helix'"));
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	/* 콜리전 설정 */
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	Stat->OnHpZero.AddUObject(this, &AEnemyBoss_Helix::SetDead);
}

void AEnemyBoss_Helix::AttackByAI()
{
	Super::AttackByAI();


}

void AEnemyBoss_Helix::DefaultAttackHitCheck()
{
	Super::DefaultAttackHitCheck();

}

void AEnemyBoss_Helix::Skill1ByAI()
{
	Super::Skill1ByAI();

}

float AEnemyBoss_Helix::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	return 0.0f;
}

float AEnemyBoss_Helix::TakeExp()
{
	Super::TakeExp();

	return 0.0f;
}

void AEnemyBoss_Helix::Stun()
{
	Super::Stun();

}

void AEnemyBoss_Helix::SetDead()
{
	Super::SetDead();


}
