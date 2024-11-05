// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/AIControllerSiege.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Animation/EnemyAnimInstance.h"
#include "Interface/AIInterface.h"
#include "Enemy/EnemyRanged_Siege.h"

AAIControllerSiege::AAIControllerSiege()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.f;
	SightConfig->LoseSightRadius = 2500.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.f;
	SightConfig->SetMaxAge(10.f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerSiege::OnPerceptionUpdated);
}

void AAIControllerSiege::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor->ActorHasTag(TEXT("Player")))
	{
		Stimulus = CanSenseActor(Actor, EAIPerceptionSense::EPS_Sight);
		HandleSenseSight(Actor, Stimulus);
	}
}

FAIStimulus AAIControllerSiege::CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense)
{
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	FAIStimulus ResultStimulus;

	AIPerception->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);

	//AI 가 지금 감지할 수 있는 감각 Sight, Damage
	TSubclassOf<UAISense> QuerySenseClass;
	switch (AIPerceptionSense)
	{
	case EAIPerceptionSense::EPS_None:
		break;
	case EAIPerceptionSense::EPS_Sight:
		QuerySenseClass = UAISense_Sight::StaticClass();
		break;
	case EAIPerceptionSense::EPS_Damage:
		QuerySenseClass = UAISense_Damage::StaticClass();
		break;
	default:
		break;
	}

	TSubclassOf<UAISense> LastSensedStimulusClass;

	for (const FAIStimulus& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
	{
		LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus);

		if (QuerySenseClass == LastSensedStimulusClass)
		{
			ResultStimulus = AIStimulus;

			return ResultStimulus;
		}
	}

	return ResultStimulus;
}

void AAIControllerSiege::HandleSenseSight(AActor* Actor, const FAIStimulus& AIStimulus)
{
	AEnemyRanged_Siege* Enemy = CastChecked<AEnemyRanged_Siege>(GetPawn());
	UEnemyAnimInstance* Anim = Cast<UEnemyAnimInstance>(Enemy->GetMesh()->GetAnimInstance());

	if (AIStimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Actor);
		Anim->bIsBattle = true;
	}
	else
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
		Anim->bIsBattle = false;
	}
}