// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/StaffDefaultAttackNotify.h"
#include "Interface/StaffInterface.h"
#include "Character/CharacterDefaultAttackComponent.h"

void UStaffDefaultAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IStaffInterface* Interface = Cast<IStaffInterface>(MeshComp->GetOwner()->GetComponentByClass<UCharacterDefaultAttackComponent>());
	if (Interface)
	{
		Interface->StaffDefaultAttack();
	}
}
