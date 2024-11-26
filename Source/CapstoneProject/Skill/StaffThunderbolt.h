// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/ProjectileBase.h"
#include "StaffThunderbolt.generated.h"

UCLASS()
class CAPSTONEPROJECT_API AStaffThunderbolt : public AProjectileBase
{
	GENERATED_BODY()
	
public:	
	AStaffThunderbolt();

protected:
	virtual void BeginPlay() override;

public:	
	void ActiveThunderbolt();

private:
	bool CheckInArea(TArray<FOverlapResult>& InOverlapResults);
	void ThunderboltDestory(class UParticleSystemComponent* PSystem);


private:
	UPROPERTY(VisibleAnywhere, Category = "Root")
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystemComponent> ParticleComponent;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TObjectPtr<class USoundWave> Sound;
};
