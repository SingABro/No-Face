// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/TankerSkill_1_Notify.h"
#include "Interface/AIInterface.h"

void UTankerSkill_1_Notify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAIInterface* Interface = Cast<IAIInterface>(MeshComp->GetOwner());
	if (Interface)
	{
		Interface->Skill_1_HitCheck();
	}
}
