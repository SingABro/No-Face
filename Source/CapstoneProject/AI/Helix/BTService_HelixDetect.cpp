// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Helix/BTService_HelixDetect.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AIInterface.h"

UBTService_HelixDetect::UBTService_HelixDetect()
{
	NodeName = TEXT("HelixDetect");
	Interval = 1.f;
}

void UBTService_HelixDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr) return;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr) return;

	IAIInterface* Interface = Cast<IAIInterface>(Owner);
	if (Interface == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Interface?"));
		return;
	}

	FVector OwnerLoc = Owner->GetActorLocation();
	FVector TargetLoc = Target->GetActorLocation();

	FVector DirectVec = TargetLoc - OwnerLoc;
	FRotator DirectRot = FRotationMatrix::MakeFromX(DirectVec).Rotator();

	if (FMath::Abs(DirectRot.Yaw - Owner->GetActorRotation().Yaw) < 15.f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanBattle"), true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanBattle"), false);
	}

	float Distance = FVector::Distance(OwnerLoc, TargetLoc);
	if (Distance >= 1000.f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanDash"), true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanDash"), false);

		if (Distance <= Interface->GetAttackInRange())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanDefaultAttack"), true);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanDefaultAttack"), false);
		}
	}
	
	/* 각 스킬 확인할 용도 디버그 */
	bool DebugFlag = false;
	if (DebugFlag)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill4"), true);
	}
	else
	{
		uint8 Index = FMath::RandRange(2, 8);
		if (Index == 2)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill2"), true);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill3"), false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill4"), false);
		}
		else if (Index == 3)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill2"), false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill3"), true);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill4"), false);
		}
		else if (Index == 4)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill2"), false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill3"), false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill4"), true);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill2"), false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill3"), false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanSkill4"), false);
		}
	}


}
