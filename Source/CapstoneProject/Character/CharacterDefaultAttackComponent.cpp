// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterDefaultAttackComponent.h"
#include "Character/CharacterComboAttackData.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Weapon/Arrow.h"
#include "Weapon/Bow.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "Skill/StaffDefaultAttackProjectile.h"
#include "Enemy/EnemyBase.h"
#include "Stat/CharacterDataStat.h"

UCharacterDefaultAttackComponent::UCharacterDefaultAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentWeaponType = 0;

	static ConstructorHelpers::FObjectFinder<UCharacterDataStat> DataStatRef(TEXT("/Script/CapstoneProject.CharacterDataStat'/Game/No-Face/Character/Stat/DA_CharacterStatData.DA_CharacterStatData'"));
	if (DataStatRef.Object)
	{
		Stat = DataStatRef.Object;
	}
}


void UCharacterDefaultAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = CastChecked<ACharacter>(GetOwner());
}


void UCharacterDefaultAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCharacterDefaultAttackComponent::SetWeaponType(const int32& InCurrentWeaponType)
{
	CurrentWeaponType = InCurrentWeaponType;
}

void UCharacterDefaultAttackComponent::BeginAttack()
{
	if (CurrentCombo == 0)
	{
		switch (CurrentWeaponType)
		{
		case 0:
			BeginSwordDefaultAttack();
			return;
		case 1:
			BeginBowDefaultAttack();
			return;
		case 2:
			BeginStaffDefaultAttack();
			return;
		default:
			return;
		}
	}

	if (CurrentWeaponType == 0)
	{
		if (!SwordComboTimer.IsValid())
		{
			SwordHasNextComboCommand = false;
		}
		else
		{
			SwordHasNextComboCommand = true;
		}
	}
	else if (CurrentWeaponType == 2)
	{
		if (!StaffComboTimer.IsValid())
		{
			StaffHasNextComboCommand = false;
		}
		else
		{
			StaffHasNextComboCommand = true;
		}
	}
	
}

void UCharacterDefaultAttackComponent::BeginSwordDefaultAttack()
{
	bCanChangeWeapon = false;
	CurrentCombo = 1;

	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(SwordDefaultAttackMontage, 1.5f);

	FOnMontageEnded MontageEnded;
	MontageEnded.BindUObject(this, &UCharacterDefaultAttackComponent::EndSwordDefaultAttack);
	AnimInstance->Montage_SetEndDelegate(MontageEnded, SwordDefaultAttackMontage);

	SwordComboTimer.Invalidate();
	SetSwordComboTimer();
}

void UCharacterDefaultAttackComponent::EndSwordDefaultAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bCanChangeWeapon = true;
}

void UCharacterDefaultAttackComponent::SetSwordComboTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(SwordComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	float ComboEffectiveTime = (SwordComboData->EffectiveFrameCount[ComboIndex] / SwordComboData->FrameRate) / 1.5f;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(SwordComboTimer, this, &UCharacterDefaultAttackComponent::CheckSwordCombo, ComboEffectiveTime, false);
	}
}

void UCharacterDefaultAttackComponent::CheckSwordCombo()
{
	SwordComboTimer.Invalidate();
	if (SwordHasNextComboCommand)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, SwordComboData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *SwordComboData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, SwordDefaultAttackMontage);
		SetSwordComboTimer();
		SwordHasNextComboCommand = false;
	}
}

void UCharacterDefaultAttackComponent::SwordDefaultAttackHitCheck()
{
	float AttackDamage = Stat->SwordDamage;
	float AttackRange = Stat->SwordRange;
	float AttackDegree = Stat->SwordDegree;

	FColor Color = FColor::Red;

	FVector Origin = Character->GetActorLocation();
	FVector ForwardVector = Character->GetActorForwardVector();
	FCollisionQueryParams Params(NAME_None, false, Character);
	TArray<FOverlapResult> OverlapResults;

	//Range를 반지름으로 하고 캐릭터가 중심인 원 안에 Enemy Trace에 대해 Block으로 설정된 폰들을 찾는다.
	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(AttackRange), Params);
	if (bHit) //원 안에 들어 왔다면
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			FDamageEvent DamageEvent;
			//설정된 Degree 값 안에 있는지 한번 더 확인을 거친다.
			//부채꼴의 공격 판정이 만들어짐
			if (SwordDefaultAttackRadialRange(GetOwner(), OverlapResult.GetActor(), AttackDegree))
			{
				AEnemyBase* Enemy = Cast<AEnemyBase>(OverlapResult.GetActor());
				if (Enemy)
				{
					Enemy->TakeDamage(AttackDamage, DamageEvent, Character->GetController(), Character, TEXT("Default"));
				}
				Color = FColor::Green;
			}
		}
	}

	//공격 범위 라인으로 나타내기
	SwordDefaultAttackHitDebug(Origin, ForwardVector, AttackRange, Color, AttackDegree);
}

void UCharacterDefaultAttackComponent::SwordDefaultAttackEnd()
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Stop(0.5f, SwordDefaultAttackMontage);
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

bool UCharacterDefaultAttackComponent::SwordDefaultAttackRadialRange(AActor* Player, AActor* Target, float RadialAngle)
{
	if (!Player || !Target) return false;

	FVector PlayerLocation = Player->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	FVector ForwardVector = Player->GetActorForwardVector();
	FVector DirectionToTarget = (TargetLocation - PlayerLocation).GetSafeNormal();

	// 타겟이 부채꼴 각도 내에 있는지 확인
	float DotProduct = FVector::DotProduct(ForwardVector, DirectionToTarget);
	float AngleToTarget = FMath::Acos(DotProduct);

	// 라디안에서 각도로 변환
	float AngleToTargetDegrees = FMath::RadiansToDegrees(AngleToTarget);

	return AngleToTargetDegrees <= (RadialAngle / 2.0f);
}

void UCharacterDefaultAttackComponent::SwordDefaultAttackHitDebug(const FVector& Start, const FVector& ForwardVector, float AttackRange, const FColor& Color, float Degree)
{
	// 부채꼴의 두 끝점 계산
	FVector LeftVector = ForwardVector.RotateAngleAxis(-Degree / 2.0f, FVector::UpVector);
	FVector RightVector = ForwardVector.RotateAngleAxis(Degree / 2.0f, FVector::UpVector);

	FVector LeftEndpoint = Start + LeftVector * AttackRange;
	FVector RightEndpoint = Start + RightVector * AttackRange;

	// 부채꼴의 중심선
	DrawDebugLine(GetWorld(), Start, Start + ForwardVector * AttackRange, Color, false, 3.0f);

	// 부채꼴의 두 끝선
	DrawDebugLine(GetWorld(), Start, LeftEndpoint, Color, false, 3.0f);
	DrawDebugLine(GetWorld(), Start, RightEndpoint, Color, false, 3.0f);
}

void UCharacterDefaultAttackComponent::BeginBowDefaultAttack()
{
	if (!bCanBowAttack) return;
	bCanBowAttack = false;
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	
	bCanChangeWeapon = false;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(BowDefaultAttackMontage, 2.f);
	
	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UCharacterDefaultAttackComponent::EndBowDefaultAttack);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, BowDefaultAttackMontage);
}

void UCharacterDefaultAttackComponent::EndBowDefaultAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bCanChangeWeapon = true;
	bCanBowAttack = true;
}

void UCharacterDefaultAttackComponent::SetBow(ABow* InBow)
{
	Bow = InBow;
}

void UCharacterDefaultAttackComponent::StartAnimation()
{
	Bow->GetMesh()->PlayAnimation(BowPullAnim, false);

	if (Bow == nullptr) return;
	FVector SpawnLocation = Bow->GetMesh()->GetSocketLocation(TEXT("Arrow_Socket"));
	FRotator SpawnRotation = Bow->GetMesh()->GetSocketRotation(TEXT("Arrow_Socket"));

	Arrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, SpawnLocation, SpawnRotation);
	Arrow->SetOwner(Character);
	Arrow->SetActorRotation(Character->GetActorForwardVector().Rotation());
}

void UCharacterDefaultAttackComponent::EndAnimation()
{
	Bow->GetMesh()->PlayAnimation(BowIdleAnim, false);

	Arrow->Init(Character->GetActorForwardVector());
}

void UCharacterDefaultAttackComponent::BeginStaffDefaultAttack()
{
	bCanChangeWeapon = false;
	CurrentCombo = 1;
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(StaffDefaultAttackMontage, 1.5f);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UCharacterDefaultAttackComponent::EndStaffDefaultAttack);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, StaffDefaultAttackMontage);

	StaffComboTimer.Invalidate();
	SetStaffComboTimer();
}

void UCharacterDefaultAttackComponent::EndStaffDefaultAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	
	CurrentCombo = 0;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bCanChangeWeapon = true;
}

void UCharacterDefaultAttackComponent::SetStaffComboTimer()
{
	int32 Index = CurrentCombo - 1;
	ensure(StaffComboData->EffectiveFrameCount.IsValidIndex(Index));

	float EffectiveTime = (StaffComboData->EffectiveFrameCount[Index] / StaffComboData->FrameRate) / 1.5f;
	if (EffectiveTime >= 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(StaffComboTimer, this, &UCharacterDefaultAttackComponent::CheckStaffCombo, EffectiveTime, false);
	}
}

void UCharacterDefaultAttackComponent::CheckStaffCombo()
{
	StaffComboTimer.Invalidate();
	if (StaffHasNextComboCommand)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, StaffComboData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *StaffComboData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, StaffDefaultAttackMontage);
		SetStaffComboTimer();
		StaffHasNextComboCommand = false;	
	}
}

void UCharacterDefaultAttackComponent::StaffDefaultAttack()
{
	FVector OriginLoc = Character->GetActorLocation() + Character->GetActorForwardVector() * 400.f;
	FRotator OriginRot = Character->GetActorRotation();
	if (CurrentCombo == 2)
	{
		OriginLoc * 150.f;
	}
	StaffAttackPtr = GetWorld()->SpawnActor<AStaffDefaultAttackProjectile>(StaffAttackClass, OriginLoc, OriginRot);
	StaffAttackPtr->SetOwner(Character);
	StaffAttackPtr->Init(Character->GetActorForwardVector());
}

