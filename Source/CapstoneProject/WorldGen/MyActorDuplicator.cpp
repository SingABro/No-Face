#include "MyActorDuplicator.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"  // FMath ����� ���� �ʿ�

AMyActorDuplicator::AMyActorDuplicator()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMyActorDuplicator::BeginPlay()
{
    Super::BeginPlay();

    if (StaticMeshToDuplicate && RoomActorClass)
    {
        // ���� �� ���� (���� �߽�)
        FRoom StartRoom;
        StartRoom.Location = GetActorLocation();
        Rooms.Add(StartRoom);

        // �� ���� ���� (4 ����)
        CreateRooms(StartRoom, MaxDepth, EDirection::UP);
        CreateRooms(StartRoom, MaxDepth, EDirection::DOWN);
        CreateRooms(StartRoom, MaxDepth, EDirection::RIGHT);
        CreateRooms(StartRoom, MaxDepth, EDirection::LEFT);
    }
}

EDirection AMyActorDuplicator::GetRandomDirection(EDirection CurrentDir)
{
    float RandValue = FMath::FRand();
    if (RandValue < 0.5f) {
        return CurrentDir;
    }
    else if (RandValue < 0.75f) {
        return static_cast<EDirection>((static_cast<uint8>(CurrentDir) * 2) % 15);
    }
    else {
        return static_cast<EDirection>((static_cast<uint8>(CurrentDir) * 8) % 15);
    }
}

void AMyActorDuplicator::CreateRooms(FRoom& CurrentRoom, int32 Depth, EDirection CurrentDir)
{
    if (Depth == 0)
    {
        CurrentRoom.bIsEndRoom = true;
        return;
    }

    // ���ο� ���� ����
    EDirection NewDir = GetRandomDirection(CurrentDir);

    // ���ο� �� ���� �� ��ġ ���
    FRoom NextRoom;
    if (CurrentDir == EDirection::UP)
    {
        NextRoom.Location = CurrentRoom.Location + FVector(0.f, -OffsetDistance.Y, 0.f);
    }
    else if (CurrentDir == EDirection::DOWN)
    {
        NextRoom.Location = CurrentRoom.Location + FVector(0.f, OffsetDistance.Y, 0.f);
    }
    else if (CurrentDir == EDirection::RIGHT)
    {
        NextRoom.Location = CurrentRoom.Location + FVector(OffsetDistance.X, 0.f, 0.f);
    }
    else if (CurrentDir == EDirection::LEFT)
    {
        NextRoom.Location = CurrentRoom.Location + FVector(-OffsetDistance.X, 0.f, 0.f);
    }

    // �ߺ� �� üũ (�̹� ������ ��ġ���� Ȯ��)
    for (const FRoom& Room : Rooms)
    {
        if (Room.Location.Equals(NextRoom.Location))
        {
            return;
        }
    }

    // ���� �迭�� �߰�
    Rooms.Add(NextRoom);

    // ���ο� Room ���� ����
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ARoomActor* DuplicatedRoom = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, NextRoom.Location, FRotator::ZeroRotator, SpawnParams);
    if (DuplicatedRoom)
    {
        DuplicatedRoom->MeshComponent->SetStaticMesh(StaticMeshToDuplicate);
    }

    // ��������� ���� �� ����
    CreateRooms(NextRoom, Depth - 1, NewDir);
}
