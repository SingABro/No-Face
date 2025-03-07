// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AIInterface.h"
#include "Interface/EnemyHpBarWidgetInterface.h"
#include "EnemyBase.generated.h"

DECLARE_DELEGATE_OneParam(FOnDead, float /* TakeExp */)
DECLARE_DELEGATE(FOnDeath)

UCLASS()
class CAPSTONEPROJECT_API AEnemyBase : public ACharacter, public IAIInterface, public IEnemyHpBarWidgetInterface
{
	GENERATED_BODY()

public:
	AEnemyBase();

protected:
	virtual void BeginPlay() override;

/* AI 인터페이스 구현 섹션 */
public:
	FEnemyAttackFinished EnemyAttackFinished;
	FEnemySkill1Finished EnemySkill1Finished;
	
	virtual float GetPatrolRadius() override;
	virtual float GetDetectRadius() override;
	virtual float GetAttackInRange() override;
	virtual float GetTurnSpeed() override;
	virtual float GetDetectTime() override;

	virtual void SetEnemyAttackDelegate(const FEnemyAttackFinished& InEnemyAttackFinished) override;
	virtual void SetEnemySkill1Delegate(const FEnemySkill1Finished& InEnemySkill1Finished) override;
	virtual void AttackByAI() override;
	virtual void DefaultAttackHitCheck() override;

	virtual void Skill1ByAI() override;


public:
	static FOnDead OnDead;
	FOnDeath OnDeath;

/* 대미지 적용 가상함수 */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) { return -1; };
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, FName Type);

/* Widget */
	virtual void SetupHpBarWidget(class UEnemyHpBarWidget* InHpBarWidget) override;

/* 스텟 몬스터마다 리턴하는 경험치값을 다르게 설정하기 위한 가상함수 */
	virtual float TakeExp();

/* Stun 은 보스 몬스터 말고 있을 것 같으니 상속받아 구현하자 */
	virtual void Stun();

/* 경험치 나눠줄 때 SetDead에서 FOnDead 델리게이트 호출하여 송신함 */
	virtual void SetDead();


protected:
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	TObjectPtr<class UEnemyStatComponent> Stat;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> HpBarComponent;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UEnemyPtrWidget> HpBarClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UEnemyDamagedTextWidget> DamagedTextClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UEnemyDamagedTextWidget> DamagedText;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UEnemyStunTextWidget> StunTextClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UEnemyStunTextWidget> StunText;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TMap<FName, class UParticleSystem*> HitParticleCollection;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystemComponent> ImpactParticleComponent;

	bool IsDead = false;

};
