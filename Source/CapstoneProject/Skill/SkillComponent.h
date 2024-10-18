// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/SwordSkillInterface.h"
#include "Interface/BowSkillInterface.h"
#include "Interface/PlayerSkillUIInterface.h"
#include "SkillComponent.generated.h"

DECLARE_DELEGATE(FParryingSign)
DECLARE_DELEGATE(FShieldSign)

UENUM(BlueprintType)
enum class ESkillState : uint8
{
	Progress = 0,
	CanSkill
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAPSTONEPROJECT_API USkillComponent : public UActorComponent, public ISwordSkillInterface, public IBowSkillInterface, public IPlayerSkillUIInterface
{
	GENERATED_BODY()

public:	
	USkillComponent();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* 활 W, 스태프 Q, W, E 등 캐스팅이 필요한 스킬의 Flag의 Get, Set */
	FORCEINLINE bool GetCastingFlag() { return bCasting; }
	FORCEINLINE void SetCastingFlag(bool InCasting) { bCasting = InCasting; }

	/* 스킬 실행 중 무기 교체 방지 */
	FORCEINLINE bool CanChangeWeapon() { return bCanChangeWeapon; }

	/* 스킬 실행 중 기본 공격 방지 */
	FORCEINLINE ESkillState& GetSkillState() { return CurrentSkillState; }

public:
	virtual void SetupSkillUIWidget(class UHUDWidget* InHUDWidget) override;

	/* 패링 스킬이 실행되면 패링 플래그를 토글한다. */
	FParryingSign ParryingSign;

	/* 패링 성공시 실행될 함수, Attacker라는 공격을 한 적의 포인터를 인자로 받는다. */
	void ParryingSuccess(AActor* Attacker);

	/* 방어막 델리게이트 */
	FShieldSign ShieldSign;

	/* 스킬 함수들 */
	void PlaySkill_Q();
	void PlaySkill_W();
	void PlaySkill_E();
	void PlaySkill_R();

	/* 현재 무기 상태 바꿈 -> CharacterBase에서 실행 */
	void SetWeaponType(const int32& InCurrentWeaponType);

	/* 현재 캐스팅되는 스킬을 담을 컨테이너 */
	TQueue<TFunction<void()>> SkillQueue;

	
private:
	//Sword Skill Montage
	void BeginSword_Q();	//검 Q - 찌르기 시작
	void EndSword_Q(class UAnimMontage* Target, bool IsProperlyEnded);	//검 Q - 찌르기 끝
	void BeginSword_W(); //검 W - 휠윈드 시작
	void EndSword_W(class UAnimMontage* Target, bool IsProperlyEnded); //검 W - 휠윈드 끝
	void BeginSword_E(); //검 E - 패링 시작
	void EndSword_E(class UAnimMontage* Target, bool IsProperlyEnded); //검 E - 패링 끝
	void BeginSword_R(); //검 R - 검기 시작
	void EndSword_R(class UAnimMontage* Target, bool IsProperlyEnded); //검 R - 검기 끝

	/* 검 히트 체크 */
	virtual void Sword_Q_SkillHitCheck() override;
	virtual void Sword_W_SkillHitCheck() override;
	virtual void Sword_R_SkillHitCheck() override;

	
	//Bow Skill Montage
	void BeginBow_Q(); //활 Q - 전방에 여러발 날리기 (애쉬 W) 시작
	void EndBow_Q(class UAnimMontage* Target, bool IsProperlyEnded); 
	void BeginBow_W(); //활 W - 범위에 화살 뿌리기
	void EndBow_W(class UAnimMontage* Target, bool IsProperlyEnded);
	void BeginBow_E(); //활 E - 백스텝
	void EndBow_E(class UAnimMontage* Target, bool IsProperlyEnded);
	void BeginBow_R(); //활 R - 기 모았다가 쏘기
	void EndBow_R(class UAnimMontage* Target, bool IsProperlyEnded);
	void FireBow_R();

	virtual void Bow_Q_Skill() override; //활 Q 화살 소환 로직
	virtual void Bow_W_Skill() override; //활 W 애니메이션 중간에 멈추기


	//Staff Skill Montage
	void BeginStaff_Q(); //스태프 Q - 메테오 시작
	void EndStaff_Q(class UAnimMontage* Target, bool IsProperlyEnded); //스태프 Q - 메테오 끝
	void BeginStaff_W(); //스태프 W - 범위 바인딩 시작
	void EndStaff_W(class UAnimMontage* Target, bool IsProperlyEnded); //스태프 W - 범위 바인딩 끝
	void BeginStaff_E(); //스태프 E - 범위 쉴?드 시작
	void EndStaff_E(class UAnimMontage* Target, bool IsProperlyEnded); //스태프 E - 쉴?드 끝
	void BeginStaff_R(); //스태프 R - 주위 번개 공격 시작
	void EndStaff_R(class UAnimMontage* Target, bool IsProperlyEnded); //스태프 R - 주위 번개 공격 끝

	/* 스킬 몽타주 모아놓은 Primary Asset */
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UCharacterSkillMontageData> SkillMontageData;

	/* 스킬 시전할 곳 표시 하기 위한 데이터 */
	FHitResult Cursor;
	bool bCasting = false;

	/* 스텟 섹션(스킬 레벨 참조용) */
private:
	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//TObjectPtr<class UCharacterStatComponent> Stat;

/* Sword 데이터 */
private:
	UPROPERTY(EditAnywhere, Category = "SwordAura")
	TSubclassOf<class ASwordAura> SwordAuraClass;

/* Bow 데이터 */
private:
	UPROPERTY(EditAnywhere, Category = "Bow")
	TObjectPtr<class UParticleSystem> RainArrows;

	UPROPERTY(EditAnywhere, Category = "Bow")
	TSubclassOf<class AArrow> ArrowClass;

	FTimerHandle AutoTargetingTimer;

/* Staff 데이터 */
private:
	UPROPERTY(EditAnywhere, Category = "Staff")
	TSubclassOf<class AStaffMeteor> MeteorClass;

	UPROPERTY(EditAnywhere, Category = "Staff")
	TObjectPtr<class UParticleSystem> MeteorCastingEffect;

	UPROPERTY(EditAnywhere, Category = "Staff")
	TSubclassOf<class AStaffArea> AreaClass;

	UPROPERTY(EditAnywhere, Category = "Staff")
	TSubclassOf<class AStaffThunderbolt> ThunderboltClass;

	UPROPERTY(EditAnywhere, Category = "Staff")
	TObjectPtr<class UParticleSystem> ThunderboltCastingEffect;

/* 쿨타임 섹션 */
private:
	void StartCooldown(float CooldownDuration, FTimerHandle& CooldownTimerHandle, bool& bCanUseSkill, ESkillType SkillType, int32 WeaponType, float& Timer);

	UPROPERTY()
	TObjectPtr<class UHUDWidget> Widget;

	FTimerHandle CooldownTimerHandle_Sword_Q;
	FTimerHandle CooldownTimerHandle_Sword_W;
	FTimerHandle CooldownTimerHandle_Sword_E;
	FTimerHandle CooldownTimerHandle_Sword_R;

	FTimerHandle CooldownTimerHandle_Bow_Q;
	FTimerHandle CooldownTimerHandle_Bow_W;
	FTimerHandle CooldownTimerHandle_Bow_E;
	FTimerHandle CooldownTimerHandle_Bow_R;

	FTimerHandle CooldownTimerHandle_Staff_Q;
	FTimerHandle CooldownTimerHandle_Staff_W;
	FTimerHandle CooldownTimerHandle_Staff_E;
	FTimerHandle CooldownTimerHandle_Staff_R;

	bool bCanUseSkill_Sword_Q = true;
	bool bCanUseSkill_Sword_W = true;
	bool bCanUseSkill_Sword_E = true;
	bool bCanUseSkill_Sword_R = true;

	bool bCanUseSkill_Bow_Q = true;
	bool bCanUseSkill_Bow_W = true;
	bool bCanUseSkill_Bow_E = true;
	bool bCanUseSkill_Bow_R = true;

	bool bCanUseSkill_Staff_Q = true;
	bool bCanUseSkill_Staff_W = true;
	bool bCanUseSkill_Staff_E = true;
	bool bCanUseSkill_Staff_R = true;

	float Sword_Q_Timer = 0.f;
	float Sword_W_Timer = 0.f;
	float Sword_E_Timer = 0.f;
	float Sword_R_Timer = 0.f;
	float Bow_Q_Timer = 0.f;
	float Bow_W_Timer = 0.f;
	float Bow_E_Timer = 0.f;
	float Bow_R_Timer = 0.f;
	float Staff_Q_Timer = 0.f;
	float Staff_W_Timer = 0.f;
	float Staff_E_Timer = 0.f;
	float Staff_R_Timer = 0.f;

	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration_Sword_Q = 3.f;

	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration_Sword_W = 3.f;

	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration_Sword_E = 3.f;
	
	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration_Sword_R = 3.f;

	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration_Bow_Q = 3.f;

	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration_Bow_W = 3.f;

	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration_Bow_E = 3.f;

	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration_Bow_R = 3.f;

	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration_Staff_Q = 3.f;

	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration_Staff_W = 3.f;

	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration_Staff_E = 3.f;

	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration_Staff_R = 3.f;

/* 유틸리티 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Character")
	TObjectPtr<class ACharacter> Character;

	UPROPERTY(VisibleAnywhere, Category = "Character")
	TObjectPtr<class APlayerController> PlayerController;

	bool bCanChangeWeapon = true;
	int32 CurrentWeaponType = 0;

	ESkillState CurrentSkillState = ESkillState::CanSkill;


/*스킬 레벨 강화*/
public:
	FORCEINLINE void UpgradeSword_Q() { Sword_Q_Upgrade++; }
	FORCEINLINE void UpgradeSword_W() { Sword_W_Upgrade++; }
	FORCEINLINE void UpgradeSword_E() { Sword_E_Upgrade++; }
	FORCEINLINE void UpgradeSword_R() { Sword_R_Upgrade++; }

	FORCEINLINE void UpgradeBow_Q() { Bow_Q_Upgrade++; }
	FORCEINLINE void UpgradeBow_W() { Bow_W_Upgrade++; }
	FORCEINLINE void UpgradeBow_E() { Bow_E_Upgrade++; }
	FORCEINLINE void UpgradeBow_R() { Bow_R_Upgrade++; }

	FORCEINLINE void UpgradeStaff_Q() { Staff_Q_Upgrade++; }
	FORCEINLINE void UpgradeStaff_W() { Staff_W_Upgrade++; }
	FORCEINLINE void UpgradeStaff_E() { Staff_E_Upgrade++; }
	FORCEINLINE void UpgradeStaff_R() { Staff_R_Upgrade++; }


	FORCEINLINE int GetSword_QLevel() { return Sword_Q_Upgrade; }
	FORCEINLINE int GetSword_WLevel() { return Sword_W_Upgrade; }
	FORCEINLINE int GetSword_ELevel() { return Sword_E_Upgrade; }
	FORCEINLINE int GetSword_RLevel() { return Sword_R_Upgrade; }

	FORCEINLINE int GetBow_QLevel() { return Bow_Q_Upgrade; }
	FORCEINLINE int GetBow_WLevel() { return Bow_W_Upgrade; }
	FORCEINLINE int GetBow_ELevel() { return Bow_E_Upgrade; }
	FORCEINLINE int GetBow_RLevel() { return Bow_R_Upgrade; }

	FORCEINLINE int GetStaff_QLevel() { return Staff_Q_Upgrade; }
	FORCEINLINE int GetStaff_WLevel() { return Staff_W_Upgrade; }
	FORCEINLINE int GetStaff_ELevel() { return Staff_E_Upgrade; }
	FORCEINLINE int GetStaff_RLevel() { return Staff_R_Upgrade; }

private:

	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int Sword_Q_Upgrade;

	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int Sword_W_Upgrade;

	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int Sword_E_Upgrade;

	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int Sword_R_Upgrade;


	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int Bow_Q_Upgrade;

	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int Bow_W_Upgrade;

	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int Bow_E_Upgrade;

	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int Bow_R_Upgrade;


	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int Staff_Q_Upgrade;

	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int Staff_W_Upgrade;

	UPROPERTY(EditAnywhere, Category = "Upgrade")

	int Staff_E_Upgrade;

	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int Staff_R_Upgrade;


};
