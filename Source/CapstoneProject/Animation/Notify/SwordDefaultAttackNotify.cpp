// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/SwordDefaultAttackNotify.h"
#include "Interface/SwordInterface.h"
#include "Character/CharacterDefaultAttackComponent.h"

USwordDefaultAttackNotify::USwordDefaultAttackNotify()
{
}

void USwordDefaultAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ISwordInterface* Interface = Cast<ISwordInterface>(MeshComp->GetOwner()->GetComponentByClass<UCharacterDefaultAttackComponent>());
	if (Interface)
	{
		Interface->SwordDefaultAttackHitCheck();
	}
}
