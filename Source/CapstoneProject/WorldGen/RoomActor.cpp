#include "RoomActor.h"
#include "WorldGen/EnemySpawnerComponent.h"
#include "NavigationSystem.h"

ARoomActor::ARoomActor()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    EnemySpawnPointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
    EnemySpawnPointComponent->SetupAttachment(MeshComponent);

    SpawnerComponent = CreateDefaultSubobject<UEnemySpawnerComponent>(TEXT("Spanwer"));
}

void ARoomActor::BeginPlay()
{
    Super::BeginPlay();

}

void ARoomActor::SetRoomInfo(int32 InIdentity, const FVector& InLocation, bool InbIsEndRoom, bool InbIsBossRoom, bool InbIsStartRoom, int stretch)
{
    Identity = InIdentity;
    Location = InLocation;
    bIsEndRoom = InbIsEndRoom;
    bIsBossRoom = InbIsBossRoom;
    bIsStartRoom = InbIsStartRoom;
    Stretch = stretch;

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


