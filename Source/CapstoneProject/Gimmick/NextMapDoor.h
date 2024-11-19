// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NextMapDoor.generated.h"

UCLASS()
class CAPSTONEPROJECT_API ANextMapDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ANextMapDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<class UBoxComponent> BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> MeshComp;
};
