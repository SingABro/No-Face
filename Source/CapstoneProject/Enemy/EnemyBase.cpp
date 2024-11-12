// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "AI/Controller/AIControllerBase.h"
#include "Components/CapsuleComponent.h"
#include "Stat/EnemyStatComponent.h"
#include "UI/EnemyHpBarWidget.h"
#include "UI/EnemyHpBarWidgetComponent.h"
#include "UI/EnemyDamagedTextWidget.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Animation/WidgetAnimation.h"

FOnDead AEnemyBase::OnDead;

AEnemyBase::AEnemyBase()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	Stat = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("Stat"));

	HpBarComponent = CreateDefaultSubobject<UEnemyHpBarWidgetComponent>(TEXT("Widget"));
	HpBarComponent->SetupAttachment(GetMesh());
	HpBarComponent->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
	static ConstructorHelpers::FClassFinder<UEnemyPtrWidget> HpBarWidgetRef(TEXT("/Game/No-Face/UI/WBP_EenmyHpBar.WBP_EenmyHpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBarClass = HpBarWidgetRef.Class;
		HpBarComponent->SetWidgetClass(HpBarClass);
		HpBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarComponent->SetDrawSize(FVector2D(150.f, 15.f));
		HpBarComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	static ConstructorHelpers::FClassFinder<UEnemyDamagedTextWidget> DamagedTextWidgetRef(TEXT("/Game/No-Face/UI/WBP_EnemyDamagedTextWidget.WBP_EnemyDamagedTextWidget_C"));
	if (DamagedTextWidgetRef.Class)
	{
		DamagedTextClass = DamagedTextWidgetRef.Class;
	}

	ImpactParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	ImpactParticleComponent->SetupAttachment(GetMesh(), TEXT("Impact"));
	ImpactParticleComponent->bAutoActivate = false;

	/* Hit Effect */
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultHitRef(TEXT("Script/Engine.ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Minion_melee/FX/Impacts/P_Minion_Impact_Default.P_Minion_Impact_Default'"));
	if (DefaultHitRef.Object)
	{
		HitParticleCollection.Add(TEXT("Default"), DefaultHitRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Sword_Q_HitRef(TEXT("/Script/Engine.ParticleSystem'/Game/11_10Effects/Sword/Sword_Q_Hit.Sword_Q_Hit'"));
	if (Sword_Q_HitRef.Object)
	{
		HitParticleCollection.Add(TEXT("Sword_Q"), Sword_Q_HitRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Sword_W_HitRef(TEXT("/Script/Engine.ParticleSystem'/Game/11_10Effects/Sword/Sword_W_Hit.Sword_W_Hit'"));
	if (Sword_W_HitRef.Object)
	{
		HitParticleCollection.Add(TEXT("Sword_W"), Sword_W_HitRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Bow_Q_HitRef(TEXT("/Script/Engine.ParticleSystem'/Game/11_10Effects/Bow/Bow_Q_Hit.Bow_Q_Hit'"));
	if (Bow_Q_HitRef.Object)
	{
		HitParticleCollection.Add(TEXT("Bow_Q"), Bow_Q_HitRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Bow_R_HitRef(TEXT("/Script/Engine.ParticleSystem'/Game/11_10Effects/Bow/Bow_R_Hit.Bow_R_Hit'"));
	if (Bow_R_HitRef.Object)
	{
		HitParticleCollection.Add(TEXT("Bow_R"), Bow_R_HitRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Staff_DefaultAttack_HitRef(TEXT("/Script/Engine.ParticleSystem'/Game/11_10Effects/Staff/Staff_AttackHit.Staff_AttackHit'"));
	if (Staff_DefaultAttack_HitRef.Object)
	{
		HitParticleCollection.Add(TEXT("Staff_Default"), Staff_DefaultAttack_HitRef.Object);
	}

	
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

}

float AEnemyBase::GetPatrolRadius()
{
	return 800.0f;
}

float AEnemyBase::GetDetectRadius()
{
	return 400.0f;
}

float AEnemyBase::GetAttackInRange()
{
	return 200.0f;
}

float AEnemyBase::GetTurnSpeed()
{
	return 10.0f;
}

void AEnemyBase::SetEnemyAttackDelegate(const FEnemyAttackFinished& InEnemyAttackFinished)
{
	EnemyAttackFinished = InEnemyAttackFinished;
}

void AEnemyBase::SetEnemySkill1Delegate(const FEnemySkill1Finished& InEnemySkill1Finished)
{
	EnemySkill1Finished = InEnemySkill1Finished;
}

void AEnemyBase::AttackByAI()
{

}

void AEnemyBase::DefaultAttackHitCheck()
{
}

void AEnemyBase::Skill1ByAI()
{
}

float AEnemyBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, FName Type)
{
	// 대미지 UI 업데이트
	FVector2D ScreenPosition;
	FVector WorldPosition = GetActorLocation() + FVector(30.f, 0.f, 100.f); // 캐릭터 위치에서 약간 위로 띄움

	// 월드 좌표를 스크린 좌표로 변환
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (UGameplayStatics::ProjectWorldToScreen(PlayerController, WorldPosition, ScreenPosition))
	{
		// 대미지 UI 위젯 생성
		DamagedText = CreateWidget<UEnemyDamagedTextWidget>(GetWorld(), DamagedTextClass);
		if (DamagedText)
		{
			DamagedText->AddToViewport();  // 화면에 추가
			DamagedText->SetDamagedText(FString::Printf(TEXT("%.2f"), Damage));  // 대미지 값 설정
			DamagedText->SetPositionInViewport(ScreenPosition); // 스크린 좌표로 위치 설정
		}
	}

	return 0.0f;
}

void AEnemyBase::SetupHpBarWidget(UEnemyHpBarWidget* InHpBarWidget)
{
	UEnemyHpBarWidget* HpBarWidget = InHpBarWidget;
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetCurrentHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UEnemyHpBarWidget::UpdateHpBar);
	}
}

float AEnemyBase::TakeExp()
{
	//기본 10 경험치
	return 10.f;
}

void AEnemyBase::Stun()
{
	// 대미지 UI 업데이트
	FVector2D ScreenPosition;
	FVector WorldPosition = GetActorLocation() + FVector(30.f, 0.f, 100.f); // 캐릭터 위치에서 약간 위로 띄움

	// 월드 좌표를 스크린 좌표로 변환
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (UGameplayStatics::ProjectWorldToScreen(PlayerController, WorldPosition, ScreenPosition))
	{
		// 대미지 UI 위젯 생성
		DamagedText = CreateWidget<UEnemyDamagedTextWidget>(GetWorld(), DamagedTextClass);
		if (DamagedText)
		{
			DamagedText->AddToViewport();  // 화면에 추가
			DamagedText->SetDamagedText(TEXT("기절")); 
			DamagedText->SetPositionInViewport(ScreenPosition); // 스크린 좌표로 위치 설정
		}
	}
}

void AEnemyBase::SetDead()
{
	IsDead = true;

	OnDead.ExecuteIfBound(TakeExp());
}









