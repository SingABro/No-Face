// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/HelixSkill_3_Notify.h"
#include "Interface/HelixSkillInterface.h"

void UHelixSkill_3_Notify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IHelixSkillInterface* Interface = Cast<IHelixSkillInterface>(MeshComp->GetOwner());
	if (Interface)
	{
		Interface->Skill_3_HitCheck();
	}
}