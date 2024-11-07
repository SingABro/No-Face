// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/AIControllerHelix.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CharacterBase.h"

AAIControllerHelix::AAIControllerHelix()
{
	
}

ACharacter* AAIControllerHelix::GetTarget()
{
	return CastChecked<ACharacter>(GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
}

void AAIControllerHelix::BeginPlay()
{
	Super::BeginPlay();

	GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass()));
}
