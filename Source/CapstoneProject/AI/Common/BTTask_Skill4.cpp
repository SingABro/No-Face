// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Common/BTTask_Skill4.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AIInterface.h"

UBTTask_Skill4::UBTTask_Skill4()
{
	NodeName = TEXT("Skill4");
}

EBTNodeResult::Type UBTTask_Skill4::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	FEnemySkill4Finished EnemySkill4Finished;
	EnemySkill4Finished.BindLambda(
		[&]()
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("SkillEnergy"), 0.f);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	Interface->SetEnemySkill4Delegate(EnemySkill4Finished);
	Interface->Skill4ByAI();

	return EBTNodeResult::InProgress;
}
