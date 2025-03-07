// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/ProjectileBase.h"
#include "StaffArea.generated.h"

UCLASS()
class CAPSTONEPROJECT_API AStaffArea : public AProjectileBase
{
	GENERATED_BODY()
	
public:	
	AStaffArea();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


private:
	bool CheckInArea(TArray<FOverlapResult>& InOverlapResults);
	void PullToCenter(float DeltaTime);

	UFUNCTION()
	void AreaDestroy(class UParticleSystemComponent* PSystem);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystemComponent> ParticleComponent;

	UPROPERTY(EditAnywhere, Category = "LifeTime")
	float LifeTime;

	float DamageTime;

};
