// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/Sword_R_SkillNotify.h"
#include "Interface/SwordSkillInterface.h"
#include "Skill/SkillComponent.h"

USword_R_SkillNotify::USword_R_SkillNotify()
{
}

void USword_R_SkillNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ISwordSkillInterface* Interface = Cast<ISwordSkillInterface>(MeshComp->GetOwner()->GetComponentByClass<USkillComponent>());
	if (Interface)
	{
		Interface->Sword_R_SkillHitCheck();
	}
}
