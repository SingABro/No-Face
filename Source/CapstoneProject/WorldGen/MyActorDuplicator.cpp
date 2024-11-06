#include "MyActorDuplicator.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"  // FMath ����� ���� �ʿ�
#include "RoomActor.h"  // ARoomActor ����


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

        // 기존 방 배열을 복제하여 작은 스케일로 다른 위치에 추가 생성
        FVector NewOrigin = FVector(10000.f, 10000.f, 8000.f);  // 새로운 기준 좌표 설정
        float ScaleFactor = 0.2f;  // 스케일 조절 (예: 0.5로 절반 크기로 설정)
        DuplicateRoomsWithScale(NewOrigin, ScaleFactor);
    }
}

void AMyActorDuplicator::DuplicateRoomsWithScale(const FVector& NewOrigin, float ScaleFactor)
{
    for (const FRoom& Room : Rooms)
    {
        // 기존 위치를 기준으로 새로운 위치를 계산
        FVector ScaledLocation = (Room.Location - GetActorLocation()) * ScaleFactor + NewOrigin;

        // 새로운 방 생성
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        ARoomActor* ScaledRoom = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, ScaledLocation, FRotator::ZeroRotator, SpawnParams);
        if (ScaledRoom)
        {
            // 스케일 적용
            ScaledRoom->SetActorScale3D(FVector(ScaleFactor));
            ScaledRoom->MeshComponent->SetStaticMesh(StaticMeshToDuplicate);
        }
    }
}

EDirection AMyActorDuplicator::GetRandomDirection(EDirection CurrentDir)
{
    float RandValue = FMath::FRand();
    if (RandValue < 0.5f) 
    {
        return CurrentDir;
    }
    else if (RandValue < 0.75f) 
    {
        return static_cast<EDirection>((static_cast<uint8>(CurrentDir) * 2) % 15);
    }
    else 
    {
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