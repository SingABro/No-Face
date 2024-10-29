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

UCharacterDefaultAttackComponent::UCharacterDefaultAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentWeaponType = 0;
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
	AnimInstance->Montage_Play(SwordDefaultAttackMontage);

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

	float ComboEffectiveTime = (SwordComboData->EffectiveFrameCount[ComboIndex] / SwordComboData->FrameRate);
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
	const float AttackDamage = 100.f /* 수정 필요 Stat->SwordDamage*/;
	const float AttackRange = 300.f /* 수정 필요 Stat->SwordRange*/;
	const float AttackDegree = 70.f /* 수정 필요 Stat->SwordDegree*/;

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
				OverlapResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetWorld()->GetFirstPlayerController(), Character);
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
	if (Bow == nullptr) return;
	FVector SpawnLocation = Bow->GetMesh()->GetSocketLocation(TEXT("Arrow_Socket"));
	FRotator SpawnRotation = Bow->GetMesh()->GetSocketRotation(TEXT("Arrow_Socket"));

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	
	bCanChangeWeapon = false;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(BowDefaultAttackMontage, 2.f);
	
	Arrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, SpawnLocation, SpawnRotation);
	Arrow->AttachToComponent(Bow->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Arrow_Socket"));

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UCharacterDefaultAttackComponent::EndBowDefaultAttack);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, BowDefaultAttackMontage);
}

void UCharacterDefaultAttackComponent::EndBowDefaultAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bCanChangeWeapon = true;
}

void UCharacterDefaultAttackComponent::SetBow(ABow* InBow)
{
	Bow = InBow;
}

void UCharacterDefaultAttackComponent::StartAnimation()
{
	Bow->GetMesh()->PlayAnimation(BowPullAnim, true);
}

void UCharacterDefaultAttackComponent::EndAnimation()
{
	Bow->GetMesh()->PlayAnimation(BowIdleAnim, true);

	if (Bow == nullptr) return;
	FVector SpawnLocation = Bow->GetMesh()->GetSocketLocation(TEXT("Arrow_Socket")) + FVector(10.f, 0.f, 0.f);
	FRotator SpawnRotation = Bow->GetMesh()->GetSocketRotation(TEXT("Arrow_Socket"));

	Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Arrow->Init(Character->GetActorForwardVector(), SpawnLocation, SpawnRotation);
}

void UCharacterDefaultAttackComponent::BeginStaffDefaultAttack()
{
	bCanChangeWeapon = false;
	CurrentCombo = 1;
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(StaffDefaultAttackMontage);

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

	float EffectiveTime = (StaffComboData->EffectiveFrameCount[Index] / StaffComboData->FrameRate);
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

