// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Common/BTTask_PerceptionPlayer.h"
#include "AIController.h"
#include "Interface/AIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"

UBTTask_PerceptionPlayer::UBTTask_PerceptionPlayer()
{
	NodeName = TEXT("Perception Player");
}

EBTNodeResult::Type UBTTask_PerceptionPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr) return EBTNodeResult::Failed;

	IAIInterface* Interface = Cast<IAIInterface>(Owner);
	if (Interface == nullptr) EBTNodeResult::Failed;

	float DetectThreshold = Interface->GetDetectTime();
	if (DetectThreshold <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DetectTime")))
	{
		APawn* Target = Cast<APawn>(UGameplayStatics::GetActorOfClass(Owner->GetWorld(), ACharacterBase::StaticClass()));
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Target);
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("DetectTime"), 0.f);
		UE_LOG(LogTemp, Display, TEXT("Detect 성공!"));
	}

	return EBTNodeResult::Succeeded;
}
