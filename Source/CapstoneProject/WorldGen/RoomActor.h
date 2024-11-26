#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomActor.generated.h"

DECLARE_DELEGATE(FOnStageChangedDelegate);

USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper
{
    GENERATED_BODY()
    FStageChangedDelegateWrapper() {}
    FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InDelegate) : StageDelegate(InDelegate) {}
    FOnStageChangedDelegate StageDelegate;
};

UENUM(BlueprintType)
enum class EStageState : uint8
{
    READY = 0,
    FIGHT,
    NEXT
};

UCLASS()
class CAPSTONEPROJECT_API ARoomActor : public AActor
{
    GENERATED_BODY()

public:
    ARoomActor();

/* 방 생성시 정보 초기화 */
    void SetRoomInfo(int32 InIdentity, const FVector& InLocation, bool InbIsEndRoom, bool bIsBossRoom, bool bIsStartRoom, int InStretch);

/* 스테이지 */
private:
    UPROPERTY(VisibleAnywhere, Category = "Room")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UBoxComponent> StageTrigger;

    UFUNCTION()
    void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

/* 문 */
private:
    UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
    TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;
    
    UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
    TMap<FName, TObjectPtr<class UStaticMeshComponent>> ActualGates;

    UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
    TMap<FName, TObjectPtr<class UBoxComponent>> GateTriggers;

    TArray<TObjectPtr<class UBoxComponent>> GetGateOpenDirection();
    TArray<TObjectPtr<class UStaticMeshComponent>> GetGateOpen();

    UFUNCTION()
    void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void OpenGates();
    void CloseAllGates();
    void InitGates();

/* 상태 */
private:
    UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
    EStageState CurrentState;

    void SetState(EStageState InNewState);

    UPROPERTY()
    TMap<EStageState, FStageChangedDelegateWrapper> StateChangeActions;

    void SetReady();
    void SetFight();
    void SetChooseNext();

/* 몬스터 */
private:
    void DeadEnemy();

    UPROPERTY(EditAnywhere, Category = "Room")
    int32 CurrentEnemyNum = 0;

    void BossClear();

/* 맵 데이터 */
private:
    UPROPERTY(EditAnywhere, Category = "Room")
    int32 Identity;  // 방의 정체성 (비트 플래그)

    UPROPERTY(EditAnywhere, Category = "Room")
    FVector Location;  // 방의 월드 좌표

    UPROPERTY(EditAnywhere, Category = "Room")
    bool bIsEndRoom;  // 마지막 방 여부

    UPROPERTY(EditAnywhere, Category = "Room")
    bool bIsBossRoom;  // 보스 방 여부

    UPROPERTY(EditAnywhere, Category = "Room")
    bool bIsStartRoom; // 시작 방 여부

    UPROPERTY(EditAnywhere, Category = "Room")
    bool bIsClearRoom; // 클리어 했나?

    UPROPERTY(EditAnywhere, Category = "Room")
    int Stretch; // 몇 번 뻗어나갔는지
};
