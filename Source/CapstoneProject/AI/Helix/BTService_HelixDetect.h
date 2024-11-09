// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_HelixDetect.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UBTService_HelixDetect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_HelixDetect();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
