// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Common/BTTask_Skill2.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AIInterface.h"

UBTTask_Skill2::UBTTask_Skill2()
{
	NodeName = TEXT("Skill2");
}

EBTNodeResult::Type UBTTask_Skill2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* PossessedPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (PossessedPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IAIInterface* Interface = Cast<IAIInterface>(PossessedPawn);
	if (Interface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FEnemySkill2Finished EnemySkill2Finished;
	EnemySkill2Finished.BindLambda(
		[&]()
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("SkillEnergy"), 0.f);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	Interface->SetEnemySkill2Delegate(EnemySkill2Finished);
	Interface->Skill2ByAI();

	return EBTNodeResult::InProgress;
}
