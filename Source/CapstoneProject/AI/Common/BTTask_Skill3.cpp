// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Common/BTTask_Skill3.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AIInterface.h"

UBTTask_Skill3::UBTTask_Skill3()
{
	NodeName = TEXT("Skill3");
}

EBTNodeResult::Type UBTTask_Skill3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	FEnemySkill3Finished EnemySkill3Finished;
	EnemySkill3Finished.BindLambda(
		[&]()
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("SkillEnergy"), 0.f);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	Interface->SetEnemySkill3Delegate(EnemySkill3Finished);
	Interface->Skill3ByAI();

	return EBTNodeResult::InProgress;
}
