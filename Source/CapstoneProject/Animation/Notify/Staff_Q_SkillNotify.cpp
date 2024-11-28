// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/Staff_Q_SkillNotify.h"
#include "Interface/StaffSkillInterface.h"
#include "Skill/SkillComponent.h"

UStaff_Q_SkillNotify::UStaff_Q_SkillNotify()
{
}

void UStaff_Q_SkillNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IStaffSkillInterface* Interface = Cast<IStaffSkillInterface>(MeshComp->GetOwner()->GetComponentByClass<USkillComponent>());
	if (Interface)
	{
		Interface->Staff_Q_Skill();
	}
}
