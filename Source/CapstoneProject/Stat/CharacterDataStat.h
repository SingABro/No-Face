// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterDataStat.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UCharacterDataStat : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetSword_Q_Level(uint8 UpgradeLevel) { Sword_Q_Level = UpgradeLevel; }
	FORCEINLINE void SetStaff_Q_Level(uint8 UpgradeLevel) { Staff_Q_Level = UpgradeLevel; }
	FORCEINLINE void SetStaff_W_Level(uint8 UpgradeLevel) { Staff_Q_Level = UpgradeLevel; }
	FORCEINLINE void SetStaff_R_Level(uint8 UpgradeLevel) { Staff_Q_Level = UpgradeLevel; }

	uint8 Sword_Q_Level = 0;
	uint8 Staff_Q_Level = 0;
	uint8 Staff_W_Level = 0;
	uint8 Staff_R_Level = 0;

/* 일반 스텟 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
	float Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
	int32 SkillPoint;

/* 검 스텟 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	float SwordDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	float SwordRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	float SwordDegree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	float Sword_Q_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	float Sword_W_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	float Sword_W_Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	float Sword_E_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	float Sword_E_Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	float Sword_R_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	float Sword_R_Range;


/* 활 스텟 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float BowDamage;                                                                               

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float BowSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float Bow_Q_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float Bow_Q_Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float Bow_W_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float Bow_W_Range;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float Bow_E_Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float Bow_R_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float Bow_R_Range;

/* 스태프 스텟 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Staff")
	float StaffDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Staff")
	float StaffSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Staff")
	float Staff_Q_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Staff")
	float Staff_Q_Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Staff")
	float Staff_W_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Staff")
	float Staff_W_Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Staff")
	float Staff_E_Threshold;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Staff")
	float Staff_R_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Staff")
	float Staff_R_Range;


};

