// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/SwordInterface.h"
#include "Interface/BowInterface.h"
#include "Interface/StaffInterface.h"
#include "CharacterDefaultAttackComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAPSTONEPROJECT_API UCharacterDefaultAttackComponent : public UActorComponent, public ISwordInterface, public IBowInterface, public IStaffInterface
{
	GENERATED_BODY()

public:	
	UCharacterDefaultAttackComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE bool CanChangeWeapon() { return bCanChangeWeapon; }

	void SetWeaponType(const int32& InCurrentWeaponType);

	void BeginAttack();

/* Sword 기본 공격 */
private:
	void BeginSwordDefaultAttack();
	void EndSwordDefaultAttack(class UAnimMontage* Target, bool IsProperlyEnded);
	void SetSwordComboTimer();
	void CheckSwordCombo();
	virtual void SwordDefaultAttackHitCheck() override;
	virtual void SwordDefaultAttackEnd() override;

	bool SwordDefaultAttackRadialRange(AActor* Player, AActor* Target, float RadialAngle);
	void SwordDefaultAttackHitDebug(const FVector& Start, const FVector& ForwardVector, float AttackRange, const FColor& Color, float Degree);

	FTimerHandle SwordComboTimer;
	bool SwordHasNextComboCommand = false;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<class UCharacterComboAttackData> SwordComboData;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> SwordDefaultAttackMontage;

/* Bow 기본 공격 */
private:
	void BeginBowDefaultAttack();
	void EndBowDefaultAttack(class UAnimMontage* Target, bool IsProperlyEnded);
	virtual void SetBow(class ABow* InBow) override;
	virtual void StartAnimation() override;
	virtual void EndAnimation() override;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> BowDefaultAttackMontage;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class ABow> Bow;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AArrow> ArrowClass;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class AArrow> Arrow;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<class UAnimationAsset> BowPullAnim;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<class UAnimationAsset> BowIdleAnim;

	bool bCanBowAttack = true;

/* Staff 기본 공격 */
private:
	void BeginStaffDefaultAttack();
	void EndStaffDefaultAttack(class UAnimMontage* Target, bool IsProperlyEnded);
	void SetStaffComboTimer();
	void CheckStaffCombo();
	virtual void StaffDefaultAttack() override;

	FTimerHandle StaffComboTimer;
	bool StaffHasNextComboCommand = false;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<class UCharacterComboAttackData> StaffComboData;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> StaffDefaultAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<class AStaffDefaultAttackProjectile> StaffAttackClass;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<class AStaffDefaultAttackProjectile> StaffAttackPtr;

/* 유틸리티 */
private:
	int32 CurrentWeaponType;
	int32 CurrentCombo = 0;

	bool bCanChangeWeapon = true;

	UPROPERTY(VisibleAnywhere, Category = "Character")
	TObjectPtr<class ACharacter> Character;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	TObjectPtr<class UCharacterDataStat> Stat;

};
