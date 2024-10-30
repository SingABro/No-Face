// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillComponent.h"
#include "Skill/SwordAura.h"
#include "Skill/StaffMeteor.h"
#include "Skill/StaffArea.h"
#include "Skill/StaffThunderbolt.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Character/CharacterSkillMontageData.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyBase.h"
#include "Weapon/Arrow.h"
#include "UI/HUDWidget.h"
#include "Stat/CharacterStatComponent.h"
#include "Stat/CharacterDataStat.h"
#include "MotionWarpingComponent.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UCharacterDataStat> StatDataRef(TEXT("/Script/CapstoneProject.CharacterDataStat'/Game/No-Face/Character/Stat/DA_CharacterStatData.DA_CharacterStatData'"));
	if (StatDataRef.Object)
	{
		StatData = StatDataRef.Object;
	}
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = CastChecked<ACharacter>(GetOwner());
	PlayerController = CastChecked<APlayerController>(Character->GetController());
	StatComponent = Character->GetComponentByClass<UCharacterStatComponent>();
}

void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCasting)
	{
		//Cursor가 멤버변수니까 해당 위치 정보를 가지고 각 스킬 함수에서 Cursor의 위치를 참조하여 시전 범위를 생성하면됨.
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, Cursor);
		DrawDebugSphere(GetWorld(), Cursor.Location, 20.f, 32, FColor::Red, false);
	}
}

void USkillComponent::PlaySkill_Q()
{
	switch (CurrentWeaponType)
	{
	case 0:
		BeginSword_Q();
		break;
	case 1:
		BeginBow_Q();
		break;
	case 2:
		BeginStaff_Q();
		break;
	default:
		break;
	}
}

void USkillComponent::PlaySkill_W()
{
	switch (CurrentWeaponType)
	{
	case 0:
		BeginSword_W();
		break;
	case 1:
		BeginBow_W();
		break;
	case 2:
		BeginStaff_W();
		break;
	default:
		break;
	}
}

void USkillComponent::PlaySkill_E()
{
	switch (CurrentWeaponType)
	{
	case 0:
		BeginSword_E();
		break;
	case 1:
		BeginBow_E();
		break;
	case 2:
		BeginStaff_E();
		break;
	default:
		break;
	}
}

void USkillComponent::PlaySkill_R()
{
	switch (CurrentWeaponType)
	{
	case 0:
		BeginSword_R();
		break;
	case 1:
		BeginBow_R();
		break;
	case 2:
		BeginStaff_R();
		break;
	default:
		break;
	}
}

void USkillComponent::SetWeaponType(const int32& InCurrentWeaponType)
{
	CurrentWeaponType = InCurrentWeaponType;
}

int USkillComponent::GetWeaponType()
{
	return CurrentWeaponType;
}

void USkillComponent::SetupSkillUIWidget(UHUDWidget* InHUDWidget)
{
	Widget = InHUDWidget;
}

/************* 검 스킬 라인 *************/
void USkillComponent::BeginSword_Q()
{
	if (!bCanUseSkill_Sword_Q || CurrentSkillState == ESkillState::Progress)
	{
		return;
	}
	else
	{
		StartCooldown(CooldownDuration_Sword_Q, CooldownTimerHandle_Sword_Q, bCanUseSkill_Sword_Q, ESkillType::Q, CurrentWeaponType, Sword_Q_Timer);
		CurrentSkillState = ESkillState::Progress;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	bCanChangeWeapon = false;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(SkillMontageData->SwordMontages[0], 3.f);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndSword_Q);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->SwordMontages[0]);
}

void USkillComponent::EndSword_Q(UAnimMontage* Target, bool IsProperlyEnded)
{
	CurrentSkillState = ESkillState::CanSkill;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bCanChangeWeapon = true;
}

void USkillComponent::Sword_Q_SkillHitCheck()
{
	FVector SpawnLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * 100.f;
	FRotator SpawnRotation = Character->GetActorRotation();

	ASwordAura* SwordAura = GetWorld()->SpawnActor<ASwordAura>(SwordAuraClass, SpawnLocation, SpawnRotation);
	SwordAura->SetOwner(Character);
	SwordAura->Init(Character->GetActorForwardVector());
}

void USkillComponent::BeginSword_W()
{
	if (!bCanUseSkill_Sword_W || CurrentSkillState == ESkillState::Progress)
	{
		return;
	}
	else
	{
		StartCooldown(CooldownDuration_Sword_W, CooldownTimerHandle_Sword_W, bCanUseSkill_Sword_W, ESkillType::W, CurrentWeaponType, Sword_W_Timer);
		CurrentSkillState = ESkillState::Progress;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	bCanChangeWeapon = false;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(SkillMontageData->SwordMontages[1], 1.5f);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndSword_W);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->SwordMontages[1]);
}

void USkillComponent::EndSword_W(UAnimMontage* Target, bool IsProperlyEnded)
{
	CurrentSkillState = ESkillState::CanSkill;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bCanChangeWeapon = true;
}

void USkillComponent::Sword_W_SkillHitCheck()
{
	const float Damage = 200.f/* 스텟 설정 필요 Stat->Sword_W_Damage*/;
	const float Radius = 200.f/* 스텟 설정 필요 Stat->Sword_W_Range*/;

	FColor Color = FColor::Red;

	FVector Origin = Character->GetActorLocation();
	FCollisionQueryParams Params(NAME_None, true, Character);
	TArray<FOverlapResult> OverlapResults;

	float UpgradeDamage = Sword_R_Upgrade * 50.0f;

	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius), Params);
	if (bHit)
	{
		FDamageEvent DamageEvent;
		for (const auto& OverlapResult : OverlapResults)
		{
			OverlapResult.GetActor()->TakeDamage(Damage + UpgradeDamage, DamageEvent, GetWorld()->GetFirstPlayerController(), Character);
			Color = FColor::Green;
		}
	}
	DrawDebugSphere(GetWorld(), Origin, Radius, 32, Color, false, 3.f);
}

void USkillComponent::BeginSword_E()
{
	if (!bCanUseSkill_Sword_E || CurrentSkillState == ESkillState::Progress)
	{
		return;
	}
	else
	{
		StartCooldown(CooldownDuration_Sword_E, CooldownTimerHandle_Sword_E, bCanUseSkill_Sword_E, ESkillType::E, CurrentWeaponType, Sword_E_Timer);
		CurrentSkillState = ESkillState::Progress;
	}
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	bCanChangeWeapon = false;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(SkillMontageData->SwordMontages[2], 1.0f);

	//애니메이션의 시작과 끝이 패링 타이밍, Notify 를 사용해 타이밍을 조정할 필요가 있음
	ParryingSign.ExecuteIfBound();

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndSword_E);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->SwordMontages[2]);
}

void USkillComponent::EndSword_E(UAnimMontage* Target, bool IsProperlyEnded)
{
	CurrentSkillState = ESkillState::CanSkill;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	ParryingSign.ExecuteIfBound();
	bCanChangeWeapon = true;
}

void USkillComponent::ParryingSuccess(AActor* Attacker)
{
	if (AEnemyBase* Enemy = Cast<AEnemyBase>(Attacker))
	{
		Enemy->Stun();
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		
		//반격 애니메이션 혹은 추가 공격;
		AnimInstance->Montage_Play(SkillMontageData->SwordMontages[4]);
	}
}

void USkillComponent::BeginSword_R()
{
	if (!bCanUseSkill_Sword_R || CurrentSkillState == ESkillState::Progress)
	{
		return;
	}
	else
	{
		StartCooldown(CooldownDuration_Sword_R, CooldownTimerHandle_Sword_R, bCanUseSkill_Sword_R, ESkillType::R, CurrentWeaponType, Sword_R_Timer);
		CurrentSkillState = ESkillState::Progress;
	}
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	bCanChangeWeapon = false;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(SkillMontageData->SwordMontages[3], 3.0f);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndSword_R);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->SwordMontages[3]);
}

void USkillComponent::EndSword_R(UAnimMontage* Target, bool IsProperlyEnded)
{
	CurrentSkillState = ESkillState::CanSkill;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bCanChangeWeapon = true;
}

void USkillComponent::Sword_R_SkillHitCheck()
{
	
}


/************* 활 스킬 라인 *************/
void USkillComponent::BeginBow_Q()
{
	/* 현재 쿨타임이 돌았는지 or 다른 스킬이 진행중인지 검사 -> 스킬 실행 중 무기 교체 금지 -> 움직임 금지 -> 애니메이션 실행 ->
		애니메이션 끝난 후 다른 스킬 사용 가능 -> 움직임 허용 -> 무기 교체 가능 */

	if (!bCanUseSkill_Bow_Q || CurrentSkillState == ESkillState::Progress)
	{
		return;
	}
	else
	{
		StartCooldown(CooldownDuration_Bow_Q, CooldownTimerHandle_Bow_Q, bCanUseSkill_Bow_Q, ESkillType::Q, CurrentWeaponType, Bow_Q_Timer);
		CurrentSkillState = ESkillState::Progress;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	bCanChangeWeapon = false;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(SkillMontageData->BowMontages[0]);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndBow_Q);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->BowMontages[0]);
}

void USkillComponent::EndBow_Q(UAnimMontage* Target, bool IsProperlyEnded)
{
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	CurrentSkillState = ESkillState::CanSkill;
	bCanChangeWeapon = true;
}

void USkillComponent::Bow_Q_Skill()
{
	FVector ForwardVector = Character->GetActorForwardVector();

	for (int32 Degree = -60; Degree <= 60; Degree += 20)
	{
		FVector SpawnVector = ForwardVector.RotateAngleAxis(Degree, FVector::UpVector);

		FVector SpawnLocation = Character->GetActorLocation() + FVector(10.f, 0.f, 0.f);
		FRotator SpawnRotator = SpawnVector.Rotation();

		AArrow* Arrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, SpawnLocation, SpawnRotator);
		Arrow->SetOwner(Character);
		Arrow->Init(SpawnVector, SpawnLocation, SpawnRotator);
	}
}

void USkillComponent::BeginBow_W()
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	bCanChangeWeapon = false;

	if (bCasting)
	{
		StartCooldown(CooldownDuration_Bow_W, CooldownTimerHandle_Bow_W, bCanUseSkill_Bow_W, ESkillType::W, CurrentWeaponType, Bow_W_Timer);
		CurrentSkillState = ESkillState::Progress;

		bCasting = false;

		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		AnimInstance->Montage_Play(SkillMontageData->BowMontages[1], 1.0f);

		FOnMontageEnded MontageEnd;
		MontageEnd.BindUObject(this, &USkillComponent::EndBow_W);
		AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->BowMontages[1]);
	}
	else
	{
		if (!bCanUseSkill_Bow_W || CurrentSkillState == ESkillState::Progress) return;

		bCasting= true;

		SkillQueue.Enqueue([this]()
			{
				BeginBow_W();
			});
	}
}

void USkillComponent::EndBow_W(UAnimMontage* Target, bool IsProperlyEnded)
{
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	CurrentSkillState = ESkillState::CanSkill;
	bCanChangeWeapon = true;
}

void USkillComponent::Bow_W_Skill()
{
	float UpgradeDamage = Bow_W_Upgrade * 50.0f;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RainArrows, Cursor.Location, FRotator::ZeroRotator);
	UGameplayStatics::ApplyRadialDamage(GetOwner(), 50.f + UpgradeDamage, Cursor.Location, 200.f, UDamageType::StaticClass(), TArray<AActor*>(), GetOwner());
}

void USkillComponent::BeginBow_E()
{
	if (!bCanUseSkill_Bow_E || CurrentSkillState == ESkillState::Progress)
	{
		return;
	}
	else
	{
		StartCooldown(CooldownDuration_Bow_E, CooldownTimerHandle_Bow_E, bCanUseSkill_Bow_E, ESkillType::E, CurrentWeaponType, Bow_E_Timer);
		CurrentSkillState = ESkillState::Progress;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	bCanChangeWeapon = false;
	AnimInstance->Montage_Play(SkillMontageData->BowMontages[2], 4.f);

	float Upgrade = Bow_E_Upgrade * -100.0f - 500.0f;
	FVector BackstepDirection = Character->GetActorForwardVector() * Upgrade;
	Character->LaunchCharacter(BackstepDirection * 10.f, true, false);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndBow_E);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->BowMontages[2]);
}

void USkillComponent::EndBow_E(UAnimMontage* Target, bool IsProperlyEnded)
{
	CurrentSkillState = ESkillState::CanSkill;
	bCanChangeWeapon = true;
}

/* 애니메이션이 천천히 실행되고, 카메라가 줌이 되는 효과 추가 하자 */
void USkillComponent::BeginBow_R()
{
	if (!bCanUseSkill_Bow_R || CurrentSkillState == ESkillState::Progress)
	{
		return;
	}
	else
	{
		StartCooldown(CooldownDuration_Bow_R, CooldownTimerHandle_Bow_R, bCanUseSkill_Bow_R, ESkillType::R, CurrentWeaponType, Bow_R_Timer);
		CurrentSkillState = ESkillState::Progress;
	}
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	bCanChangeWeapon = false;

	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	Bow_R_MotionWarpSet();
	AnimInstance->Montage_Play(SkillMontageData->BowMontages[3]);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndBow_R);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->BowMontages[3]);
}

void USkillComponent::EndBow_R(UAnimMontage* Target, bool IsProperlyEnded)
{
	CurrentSkillState = ESkillState::CanSkill;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetMotionWarpComponent()->RemoveAllWarpTargets();
	bCanChangeWeapon = true;
}

void USkillComponent::Bow_R_Skill()
{
	TArray<FHitResult> HitResults;

	const float Damage = 1000.f;
	const float Range = 500.f;
	FVector Origin = Character->GetActorLocation();
	FVector ForwardVector = Character->GetActorForwardVector() * Range;

	//사선 -45도 각도로 꽂히도록 설정
	FVector End = ForwardVector.RotateAngleAxis(-45.f, FVector::RightVector);


	FQuat RootRot = FQuat::MakeFromEuler(FVector(-45.f, 0.f, 0.f));


	FVector BoxExtent = FVector(100.f, 100.f, 100.f);
	FCollisionQueryParams Params(NAME_None, true, Character);

	float UpgradeDamage = Bow_R_Upgrade * 50.0f;

	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Origin, End, RootRot, ECC_GameTraceChannel2, FCollisionShape::MakeBox(BoxExtent), Params);
	if (bHit)
	{
		FDamageEvent DamageEvent;
		for (const auto& HitResult : HitResults)
		{
			HitResult.GetActor()->TakeDamage(Damage + UpgradeDamage, DamageEvent, PlayerController, Character);
		}
	}

	DrawDebugBox(GetWorld(), Origin, BoxExtent, RootRot, FColor::Green, false, 5.f);
	DrawDebugBox(GetWorld(), End, BoxExtent, RootRot, FColor::Green, false, 5.f);
}

/************* 지팡이 스킬 라인 *************/
void USkillComponent::BeginStaff_Q()
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	bCanChangeWeapon = false;

	if (bCasting)
	{
		StartCooldown(CooldownDuration_Staff_Q, CooldownTimerHandle_Staff_Q, bCanUseSkill_Staff_Q, ESkillType::Q, CurrentWeaponType, Staff_Q_Timer);
		CurrentSkillState = ESkillState::Progress;

		bCasting = false;

		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		AnimInstance->Montage_Play(SkillMontageData->StaffMontages[0]);

		FOnMontageEnded MontageEnd;
		MontageEnd.BindUObject(this, &USkillComponent::EndStaff_Q);
		AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->StaffMontages[0]);
	}
	else
	{
		if (!bCanUseSkill_Staff_Q || CurrentSkillState == ESkillState::Progress) return;

		bCasting = true;

		//컨테이너에 람다형식으로 함수 등록
		SkillQueue.Enqueue([this]()
			{
				BeginStaff_Q();
			});
	}
}

void USkillComponent::EndStaff_Q(UAnimMontage* Target, bool IsProperlyEnded)
{
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	CurrentSkillState = ESkillState::CanSkill;
	bCanChangeWeapon = true;
}

void USkillComponent::Staff_Q_Skill()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorCastingEffect, Cursor.Location, FRotator::ZeroRotator);

	FTimerHandle StaffQHandle;
	GetWorld()->GetTimerManager().SetTimer(StaffQHandle,
		[&]()
		{
			AStaffMeteor* Meteor = GetWorld()->SpawnActor<AStaffMeteor>(MeteorClass, Cursor.Location + FVector(0.f, 0.f, 800.f), FRotator::ZeroRotator);
			Meteor->Init(Cursor.Location);
		}, 2.f, false);
}

void USkillComponent::BeginStaff_W()
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	bCanChangeWeapon = false;

	if (bCasting)
	{
		StartCooldown(CooldownDuration_Staff_W, CooldownTimerHandle_Staff_W, bCanUseSkill_Staff_W, ESkillType::W, CurrentWeaponType, Staff_W_Timer);
		CurrentSkillState = ESkillState::Progress;

		bCasting = false;
		
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		Staff_W_MotionWarpSet();
		AnimInstance->Montage_Play(SkillMontageData->StaffMontages[1]);

		FOnMontageEnded MontageEnd;
		MontageEnd.BindUObject(this, &USkillComponent::EndStaff_W);
		AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->StaffMontages[1]);
	}
	else
	{
		if (!bCanUseSkill_Staff_W || CurrentSkillState == ESkillState::Progress) return;

		bCasting = true;

		//컨테이너에 람다형식으로 함수 등록
		SkillQueue.Enqueue([this]()
			{
				BeginStaff_W();
			});
	}
}

void USkillComponent::EndStaff_W(UAnimMontage* Target, bool IsProperlyEnded)
{
	CurrentSkillState = ESkillState::CanSkill;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetMotionWarpComponent()->RemoveAllWarpTargets();
	bCanChangeWeapon = true;
}

void USkillComponent::Staff_W_Skill()
{
	FVector SpawnLocation = Cursor.Location;
	AStaffArea* Area = GetWorld()->SpawnActor<AStaffArea>(AreaClass, SpawnLocation, FRotator::ZeroRotator);
	Area->SetOwner(Character);
}

void USkillComponent::BeginStaff_E()
{
	if (!bCanUseSkill_Staff_E) return;
	else StartCooldown(CooldownDuration_Staff_E, CooldownTimerHandle_Staff_E, bCanUseSkill_Staff_E, ESkillType::E, CurrentWeaponType, Staff_E_Timer);

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(SkillMontageData->StaffMontages[2]);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndStaff_E);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->StaffMontages[2]);

	ShieldSign.ExecuteIfBound();
}

void USkillComponent::EndStaff_E(UAnimMontage* Target, bool IsProperlyEnded)
{
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void USkillComponent::BeginStaff_R()
{
	if (!bCanUseSkill_Staff_R) return;
	else StartCooldown(CooldownDuration_Staff_R, CooldownTimerHandle_Staff_R, bCanUseSkill_Staff_R, ESkillType::R, CurrentWeaponType, Staff_R_Timer);

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(SkillMontageData->StaffMontages[3]);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndStaff_R);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->StaffMontages[3]);

	FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(TEXT("root"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ThunderboltCastingEffect, Character->GetMesh()->GetSocketLocation(TEXT("root")), FRotator::ZeroRotator);
	FTimerHandle StaffRHandle;
	GetWorld()->GetTimerManager().SetTimer(StaffRHandle,
		[&]()
		{
			AStaffThunderbolt* Thunderbolt = GetWorld()->SpawnActor<AStaffThunderbolt>(ThunderboltClass, Character->GetMesh()->GetSocketLocation(TEXT("root")), FRotator::ZeroRotator);
			Thunderbolt->SetOwner(Character);
			Thunderbolt->ActiveThunderbolt();
		}, 2.f, false);

}

void USkillComponent::EndStaff_R(UAnimMontage* Target, bool IsProperlyEnded)
{
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void USkillComponent::Bow_R_MotionWarpSet()
{
	FVector Origin = Character->GetActorLocation();
	FVector Target = (Origin + FVector(0.f, 0.f, 70.f)) + Character->GetActorForwardVector() * -250.f;
	GetMotionWarpComponent()->AddOrUpdateWarpTargetFromLocation(TEXT("BowR"), Target);
}

void USkillComponent::Staff_W_MotionWarpSet()
{
	FVector Origin = Character->GetActorLocation();
	FVector Target = Origin + Character->GetActorUpVector() * 200.f;
	GetMotionWarpComponent()->AddOrUpdateWarpTargetFromLocation(TEXT("StaffW"), Target);
}

void USkillComponent::BeginDash()
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(SkillMontageData->DashMontages[0]);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndDash);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->DashMontages[0]);
}

void USkillComponent::EndDash(UAnimMontage* Target, bool IsProperlyEnded)
{
}


void USkillComponent::StartCooldown(float CooldownDuration, FTimerHandle& CooldownTimerHandle, bool& bCanUseSkill, ESkillType SkillType, int32 WeaponType, float& Timer)
{
	bCanUseSkill = false;

	Widget->SetMaxCooldown(CooldownDuration, CurrentWeaponType, SkillType);
	Widget->StartCooldown(CurrentWeaponType, SkillType);
	Widget->UpdateCooldownBar(CooldownDuration, CooldownTimerHandle, bCanUseSkill, SkillType, WeaponType, Timer);
}

UMotionWarpingComponent* USkillComponent::GetMotionWarpComponent()
{
	return Character->GetComponentByClass<UMotionWarpingComponent>();
}

void USkillComponent::UsePlayerSkillPoint(int WeaponType, int SkillType)
{
	int SkillPoint = StatComponent->GetCurrentSkillPoint();
	if (SkillPoint > 0)	SkillPoint -= 1;
	StatComponent->SetCurrentSkillPoint(SkillPoint);
	if (WeaponType == 0)
	{
		if (SkillType == 1)
		{
			Sword_Q_Upgrade++;
		}
		else if (SkillType == 2)
		{
			Sword_W_Upgrade++;
		}
		else if (SkillType == 3)
		{
			Sword_E_Upgrade++;
		}
		else if (SkillType == 4)
		{
			Sword_R_Upgrade++;
			float UpgradeDamage = 150.0f + Sword_R_Upgrade * 50.0f;
			StatData->SetSword_R_Damage(UpgradeDamage);
		}
	}
	else if (WeaponType == 1)
	{
		if (SkillType == 1)
		{
			Bow_Q_Upgrade++;
			float UpgradeDamage = 50.0f + Bow_Q_Upgrade * 50.0f;
			StatData->SetBowDamage(UpgradeDamage);
		}
		else if (SkillType == 2)
		{
			Bow_W_Upgrade++;
		}
		else if (SkillType == 3)
		{
			Bow_E_Upgrade++;
		}
		else if (SkillType == 4)
		{
			Bow_R_Upgrade++;
		}
	}
	else if (WeaponType == 2)
	{
		if (SkillType == 1)
		{
			Staff_Q_Upgrade++;
			float UpgradeDamage = 100.0f + Staff_Q_Upgrade * 50.0f;
			StatData->SetStaff_Q_Damage(UpgradeDamage);
		}
		else if (SkillType == 2)
		{
			Staff_W_Upgrade++;
			float UpgradeDamage = 100.0f + Staff_W_Upgrade * 50.0f;
			StatData->SetStaff_W_Damage(UpgradeDamage);
		}
		else if (SkillType == 3)
		{
			Staff_E_Upgrade++;
		}
		else if (SkillType == 4)
		{
			Staff_R_Upgrade++;
			float UpgradeDamage = 100.0f + Staff_R_Upgrade * 50.0f;
			StatData->SetStaff_R_Damage(UpgradeDamage);
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Skill Point: %d"), SkillPoint);
}

int USkillComponent::GetSkillUpgradeLevel(int WeaponType, int SkillType)
{
	int SkillLevel;
	if (WeaponType == 0)
	{
		if (SkillType == 1)
		{
			SkillLevel = Sword_Q_Upgrade;
		}
		else if (SkillType == 2)
		{
			SkillLevel = Sword_W_Upgrade;
		}
		else if (SkillType == 3)
		{
			SkillLevel = Sword_E_Upgrade;
		}
		else if (SkillType == 4)
		{
			SkillLevel = Sword_R_Upgrade;
		}
	}
	else if (WeaponType == 1)
	{
		if (SkillType == 1)
		{
			SkillLevel = Bow_Q_Upgrade;
		}
		else if (SkillType == 2)
		{
			SkillLevel = Bow_W_Upgrade;
		}
		else if (SkillType == 3)
		{
			SkillLevel = Bow_E_Upgrade;
		}
		else if (SkillType == 4)
		{
			SkillLevel = Bow_R_Upgrade;
		}
	}
	else if (WeaponType == 2)
	{
		if (SkillType == 1)
		{
			SkillLevel = Staff_Q_Upgrade;
		}
		else if (SkillType == 2)
		{
			SkillLevel = Staff_W_Upgrade;
		}
		else if (SkillType == 3)
		{
			SkillLevel = Staff_E_Upgrade;
		}
		else if (SkillType == 4)
		{
			SkillLevel = Staff_R_Upgrade;
		}
	}
	return SkillLevel;
}

void USkillComponent::PlusSkillPoint()
{
	int SkillPoint = StatComponent->GetCurrentSkillPoint();
	SkillPoint += 1;
	StatComponent->SetCurrentSkillPoint(SkillPoint);
}

