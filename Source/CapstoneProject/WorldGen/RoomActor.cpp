#include "RoomActor.h"
#include "WorldGen/EnemySpawnerComponent.h"
#include "NavigationSystem.h"
#include "Components/BoxComponent.h"

ARoomActor::ARoomActor()
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    EnemySpawnPointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
    EnemySpawnPointComponent->SetupAttachment(MeshComponent);

    SpawnerComponent = CreateDefaultSubobject<UEnemySpawnerComponent>(TEXT("Spanwer"));

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

}

void ARoomActor::SetRoomInfo(int32 InIdentity, const FVector& InLocation, bool InbIsEndRoom, bool InbIsBossRoom, bool InbIsStartRoom, int InStretch)
{
    Identity = InIdentity;
    Location = InLocation;
    bIsEndRoom = InbIsEndRoom;
    bIsBossRoom = InbIsBossRoom;
    bIsStartRoom = InbIsStartRoom;
    Stretch = InStretch;

    SpawnEnemy();
}

void ARoomActor::SpawnEnemy()
{
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
    FNavLocation SpawnRandomPos;

    for (int i = 0; i < 3; ++i)
    {
        if (NavSystem->GetRandomPointInNavigableRadius(Location + FVector(0.f, 0.f, 100.f), 500, SpawnRandomPos))
        {
            SpawnRandomPos.Location.Z = Location.Z + 100.f;
            SpawnerComponent->GenerateEnemy(bIsBossRoom, 0, SpawnRandomPos);
        }
    }
}

void ARoomActor::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //SpawnEnemy(); 이때 하면 될듯;
    SetState(EStageState::FIGHT);
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

    }

    return Answer;
}

void ARoomActor::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ARoomActor::OpenGates()
{
    for (auto Gate : Gates)
    {
        (Gate.Value)->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    }
}

void ARoomActor::CloseAllGates()
{
    for (auto Gate : Gates)
    {
        (Gate.Value)->SetRelativeRotation(FRotator::ZeroRotator);
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
    for (auto GateTrigger : GateTriggers)
    {
        (GateTrigger.Value)->SetCollisionProfileName(TEXT("NoCollision"));
    }

    OpenGates();
}

void ARoomActor::SetFight()
{
    StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
    for (auto GateTrigger : GateTriggers)
    {
        (GateTrigger.Value)->SetCollisionProfileName(TEXT("NoCollision"));
    }

    CloseAllGates();
}

void ARoomActor::SetChooseNext()
{
    StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
    for (auto GateTrigger : GateTriggers)
    {
        (GateTrigger.Value)->SetCollisionProfileName(TEXT("StageTrigger"));
    }

    OpenGates();
}



