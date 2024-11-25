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
	return Cast<ACharacter>(GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
}

void AAIControllerHelix::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle StartDelay;
	GetWorld()->GetTimerManager().SetTimer(StartDelay, [&]()
		{
			GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass())));
		}, 1.f, false);
}

void AAIControllerHelix::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}
