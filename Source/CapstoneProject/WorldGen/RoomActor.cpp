#include "RoomActor.h"

ARoomActor::ARoomActor()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void ARoomActor::SetRoomInfo(int32 InIdentity, const FVector& InLocation, bool InbIsEndRoom)
{
    Identity = InIdentity;
    Location = InLocation;
    bIsEndRoom = InbIsEndRoom;
}

