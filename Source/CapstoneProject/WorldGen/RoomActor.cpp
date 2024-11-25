#include "RoomActor.h"
#include "NavigationSystem.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CharacterBase.h"
#include "WorldGen/EnemySpawner.h"
#include "Enemy/EnemyBase.h"

ARoomActor::ARoomActor()
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
    StageTrigger->SetBoxExtent(FVector(775.0, 775.0f, 300.0f));
    StageTrigger->SetupAttachment(MeshComponent);
    StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
    StageTrigger->SetCollisionProfileName(TEXT("StageTrigger"));
    StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARoomActor::OnStageTriggerBeginOverlap);

    // Gate Section
    static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };
    static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Gate/SM_GATE.SM_GATE'"));
    for (FName GateSocket : GateSockets)
    {
        UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
        Gate->SetStaticMesh(GateMeshRef.Object);
        Gate->SetupAttachment(MeshComponent, GateSocket);
        Gate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
        Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
        Gates.Add(GateSocket, Gate);

        FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
        UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
        GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
        GateTrigger->SetupAttachment(MeshComponent, GateSocket);
        GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
        GateTrigger->SetCollisionProfileName(TEXT("StageTrigger"));
        GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARoomActor::OnGateTriggerBeginOverlap);
        GateTrigger->ComponentTags.Add(GateSocket);

        GateTriggers.Add(GateSocket, GateTrigger);
    }

    // State Section
    CurrentState = EStageState::READY;
    StateChangeActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ARoomActor::SetReady)));
    StateChangeActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ARoomActor::SetFight)));
    StateChangeActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ARoomActor::SetChooseNext)));

    SetState(CurrentState);

}

void ARoomActor::SetRoomInfo(int32 InIdentity, const FVector& InLocation, bool InbIsEndRoom, bool InbIsBossRoom, bool InbIsStartRoom, int InStretch)
{
    Identity = InIdentity;
    Location = InLocation;
    bIsEndRoom = InbIsEndRoom;
    bIsBossRoom = InbIsBossRoom;
    bIsStartRoom = InbIsStartRoom;
    Stretch = InStretch;

    InitGates();
}

void ARoomActor::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
    GetGameInstance()->GetSubsystem<UEnemySpawner>()->GenerateEnemy(bIsBossRoom, Stretch, Location);

    if (bIsClearRoom == false)
    {
        SetState(EStageState::FIGHT);
    }

    TArray<AActor*> Enemys;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), Enemys);
    CurrentEnemyNum = Enemys.Num();

    for (const auto& Enemy : Enemys)
    {
        AEnemyBase* EnemyBase = Cast<AEnemyBase>(Enemy);
        EnemyBase->OnDeath.BindUObject(this, &ARoomActor::DeadEnemy);
    }
    UE_LOG(LogTemp, Display, TEXT("생성된 몬스터 수 : %d"), CurrentEnemyNum);
}

TArray<TObjectPtr<class UBoxComponent>> ARoomActor::GetGateOpenDirection()
{
    TArray<TObjectPtr<UBoxComponent>> Answer;

    switch (Identity)
    {
    case 0:
        Answer.Add(GateTriggers[TEXT("+XGate")]);
        Answer.Add(GateTriggers[TEXT("-XGate")]);
        Answer.Add(GateTriggers[TEXT("+YGate")]);
        Answer.Add(GateTriggers[TEXT("-YGate")]);
        break;
    case 1:
        Answer.Add(GateTriggers[TEXT("-YGate")]);
        break;
    case 2:
        Answer.Add(GateTriggers[TEXT("+XGate")]);
        break;
    case 3:
        Answer.Add(GateTriggers[TEXT("+XGate")]);
        Answer.Add(GateTriggers[TEXT("-YGate")]);
        break;
    case 4:
        Answer.Add(GateTriggers[TEXT("+YGate")]);
        break;
    case 5:
        Answer.Add(GateTriggers[TEXT("+YGate")]);
        Answer.Add(GateTriggers[TEXT("-YGate")]);
        break;
    case 6:
        Answer.Add(GateTriggers[TEXT("+XGate")]);
        Answer.Add(GateTriggers[TEXT("+YGate")]);
        break;
    case 7:
        Answer.Add(GateTriggers[TEXT("+XGate")]);
        Answer.Add(GateTriggers[TEXT("+YGate")]);
        Answer.Add(GateTriggers[TEXT("-YGate")]);
        break;
    case 8:
        Answer.Add(GateTriggers[TEXT("-XGate")]);
        break;
    case 9:
        Answer.Add(GateTriggers[TEXT("-XGate")]);
        Answer.Add(GateTriggers[TEXT("-YGate")]);
        break;
    case 10:
        Answer.Add(GateTriggers[TEXT("+XGate")]);
        Answer.Add(GateTriggers[TEXT("-XGate")]);
        break;
    case 11:
        Answer.Add(GateTriggers[TEXT("+XGate")]);
        Answer.Add(GateTriggers[TEXT("-XGate")]);
        Answer.Add(GateTriggers[TEXT("+YGate")]);
        break;
    case 12:
        Answer.Add(GateTriggers[TEXT("-XGate")]);
        Answer.Add(GateTriggers[TEXT("+YGate")]);
        break;
    case 13:
        Answer.Add(GateTriggers[TEXT("-XGate")]);
        Answer.Add(GateTriggers[TEXT("+YGate")]);
        Answer.Add(GateTriggers[TEXT("-YGate")]);
        break;
    case 14:
        Answer.Add(GateTriggers[TEXT("+XGate")]);
        Answer.Add(GateTriggers[TEXT("-XGate")]);
        Answer.Add(GateTriggers[TEXT("+YGate")]);
        break;
    case 15:
        Answer.Add(GateTriggers[TEXT("+XGate")]);
        Answer.Add(GateTriggers[TEXT("-XGate")]);
        Answer.Add(GateTriggers[TEXT("+YGate")]);
        Answer.Add(GateTriggers[TEXT("-YGate")]);
        break;
    }

    return Answer;
}

void ARoomActor::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    check(OverlappedComponent->ComponentTags.Num() == 1);
    FName ComponentTag = OverlappedComponent->ComponentTags[0];
    FName SocketName = FName(*ComponentTag.ToString().Left(2));

    bIsClearRoom = true;

    FVector NewLocation = FVector::ZeroVector;
    if (OverlappedComponent->ComponentHasTag(TEXT("+XGate")))
    {
        NewLocation = FVector::ForwardVector;
    }
    else if (OverlappedComponent->ComponentHasTag(TEXT("-XGate")))
    {
        NewLocation = -FVector::ForwardVector;
    }
    else if (OverlappedComponent->ComponentHasTag(TEXT("+YGate")))
    {
        NewLocation = FVector::RightVector;
    }
    else if (OverlappedComponent->ComponentHasTag(TEXT("-YGate")))
    {
        NewLocation = -FVector::RightVector;
    }

    ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass()));  
    Player->SetActorLocation(GetActorLocation() + NewLocation * 8000.f + FVector(0.f, 0.f, 150.f));
    Player->StopMove();
    Player->OnWarpNextMap.ExecuteIfBound(NewLocation);
}

void ARoomActor::OpenGates()
{
    for (const auto& Gate : Gates)
    {
        (Gate.Value)->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    }
}

void ARoomActor::CloseAllGates()
{
    for (const auto& Gate : Gates)
    {
        (Gate.Value)->SetRelativeRotation(FRotator::ZeroRotator);
    }
}

void ARoomActor::InitGates()
{
    for (const auto& GateTrigger : GetGateOpenDirection())
    {
        GateTrigger->SetCollisionProfileName(TEXT("StageTrigger"));
    }
}

void ARoomActor::SetState(EStageState InNewState)
{
    CurrentState = InNewState;

    if (StateChangeActions.Contains(InNewState))
    {
        StateChangeActions[CurrentState].StageDelegate.ExecuteIfBound();
    }
}

void ARoomActor::SetReady()
{
    StageTrigger->SetCollisionProfileName(TEXT("StageTrigger"));
    for (const auto& GateTrigger : GetGateOpenDirection())
    {
        GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
    }

    OpenGates();
}

void ARoomActor::SetFight()
{
    StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
    for (const auto& GateTrigger : GetGateOpenDirection())
    {
        GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
    }

    CloseAllGates();
}

void ARoomActor::SetChooseNext()
{
    StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
    for (const auto& GateTrigger : GetGateOpenDirection())
    {
        GateTrigger->SetCollisionProfileName(TEXT("StageTrigger"));
    }

    OpenGates();
}

void ARoomActor::DeadEnemy()
{
    CurrentEnemyNum--;
    if (CurrentEnemyNum == 0)
    {
        SetState(EStageState::NEXT);
        return;
    }
}



