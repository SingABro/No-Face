// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/ProjectileBase.h"
#include "StaffMeteor.generated.h"

UCLASS()
class CAPSTONEPROJECT_API AStaffMeteor : public AProjectileBase
{
	GENERATED_BODY()
	
public:	
	AStaffMeteor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void MeteorDestroy(class UParticleSystemComponent* PSystem);

	void Init(const FVector& InDestination);

private:
	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystemComponent> ParticleComponent;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystem> CrashEffect;

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
	float MoveSpeed;

	FVector Destination;
	bool bStart;
	

};
