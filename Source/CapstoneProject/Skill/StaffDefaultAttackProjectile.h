// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/ProjectileBase.h"
#include "StaffDefaultAttackProjectile.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API AStaffDefaultAttackProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	AStaffDefaultAttackProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Init(const FVector& InMoveDirection);

private:
	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<class UStaticMesh> Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
	TObjectPtr<class UParticleSystemComponent> ParticleComponent;

	FVector MoveDirection;

	UPROPERTY(EditAnywhere, Category = "Var")
	float LifeTime;
};
