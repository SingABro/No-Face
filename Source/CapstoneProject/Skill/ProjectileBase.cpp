// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/ProjectileBase.h"
#include "Stat/CharacterDataStat.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	static ConstructorHelpers::FObjectFinder<UCharacterDataStat> StatRef(TEXT("/Script/CapstoneProject.CharacterDataStat'/Game/No-Face/Character/Stat/DA_CharacterStatData.DA_CharacterStatData'"));
	if (StatRef.Object)
	{
		Stat = StatRef.Object;
	}

}
