// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MinimapFollow.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API AMinimapFollow : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMinimapFollow();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void WarpEvent(const FVector& InWarpLocation);

	UPROPERTY()
	FVector NewLocation;

protected:
	virtual void BeginPlay() override;
	void Init();

private:
	UPROPERTY()
	TObjectPtr<class ACharacterBase> Player;

	int32 PlusXWarpIndex = 0;
	int32 MinusXWarpIndex = 0;
	int32 PlusYWarpIndex = 0;
	int32 MinusYWarpIndex = 0;
};
