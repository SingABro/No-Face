// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class CAPSTONEPROJECT_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	TObjectPtr<class UCharacterDataStat> Stat;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystem> Particle;
};
