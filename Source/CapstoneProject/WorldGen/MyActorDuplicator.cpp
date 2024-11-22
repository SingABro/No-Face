#include "MyActorDuplicator.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "RoomActor.h"  // ARoomActor

AMyActorDuplicator::AMyActorDuplicator()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMyActorDuplicator::BeginPlay()
{
    Super::BeginPlay();

    if (RoomActorClass)
    {
        EDirection BossRoomDirection = WhereIsBossRoom();
        // 시작 방 설정
        FRoom StartRoom;
        StartRoom.Location = FVector3d(0, 0, 0);
        StartRoom.Identity = 0;
        StartRoom.bIsStartRoom = true;
        // 시작 좌표
        FIntPoint StartCoords = FIntPoint(0, 0);

        // 트리 형태의 방 생성
        do
        {
            WorldMap.Reset();
            WorldMap.Add(StartCoords, StartRoom);
            CreateRooms(StartCoords, MaxDepth, EDirection::UP, EDirection::UP, BossRoomDirection);
            CreateRooms(StartCoords, MaxDepth, EDirection::DOWN, EDirection::DOWN, BossRoomDirection);
            CreateRooms(StartCoords, MaxDepth, EDirection::RIGHT, EDirection::RIGHT, BossRoomDirection);
            CreateRooms(StartCoords, MaxDepth, EDirection::LEFT, EDirection::LEFT, BossRoomDirection);
        } while (WorldMap.Num() > 25);

        BuildActualStage(WorldMap);
    }
}

void AMyActorDuplicator::CreateRooms(const FIntPoint& CurrentCoords, int32 Depth, EDirection CurrentDir, EDirection StartDir, EDirection BossDir)
{
    if (Depth == 0)
    {
        return;
    };

    // 새 좌표 계산
    FIntPoint NewCoords = CurrentCoords;
    if (CurrentDir == EDirection::UP) NewCoords.Y -= 1;
    else if (CurrentDir == EDirection::DOWN) NewCoords.Y += 1;
    else if (CurrentDir == EDirection::RIGHT) NewCoords.X += 1;
    else if (CurrentDir == EDirection::LEFT) NewCoords.X -= 1;

    // 중복 체크 -> 0,0 체크
    // if (WorldMap.Contains(NewCoords)) return;
    if (NewCoords == FIntPoint(0, 0)) return;

    // 랜덤 방향 선택
    EDirection NewDir = GetRandomDirection(CurrentDir);

    // 새 방 생성 및 우선 저장
    FRoom NewRoom;
    NewRoom.Location = GridToWorld(NewCoords);
    NewRoom.Identity = (static_cast<int32>(CurrentDir) * 4) % 15 + (static_cast<int32>(NewDir));
    NewRoom.bIsEndRoom = false;
    NewRoom.bIsBossRoom = false;
    NewRoom.stretch = MaxDepth - Depth + 1; // 몇 번 뻗어나갔는지 저장
    if (WorldMap.Find(NewCoords) != NULL)
    { // 이미 월드에 존재하면
        FRoom tmpRoom;
        WorldMap.RemoveAndCopyValue(NewCoords, tmpRoom); // 기존 월드에 같은 좌표의 맵을 불러오고 삭제
        tmpRoom.Identity = tmpRoom.Identity | NewRoom.Identity; // 두 Identity를 합침
        tmpRoom.Location = GridToWorld(NewCoords);
        tmpRoom.bIsEndRoom = false;
        tmpRoom.bIsBossRoom = false;
        if (Depth == 1)
        {
            tmpRoom.bIsEndRoom = true;
            if (StartDir == BossDir) tmpRoom.bIsBossRoom = true;
        }
        WorldMap.Add(NewCoords, tmpRoom); // wolrd에 다시 저장
    }
    else
    {
        if (Depth == 1)
        {
            NewRoom.bIsEndRoom = true;
            if (StartDir == BossDir) NewRoom.bIsBossRoom = true;
        }
        WorldMap.Add(NewCoords, NewRoom);
    }

    // 재귀 호출
    CreateRooms(NewCoords, Depth - 1, NewDir, StartDir, BossDir);
}


// 방 Actor 생성
void AMyActorDuplicator::BuildActualStage(TMap<FIntPoint, FRoom> WMap)
{
    bool BossRoomExist = false; // 보스 방이 있는지 확인

    for (auto& Elem : WMap) // WMap의 모든 요소에 대해 반복
    {
        FRoom* tmpRoom = &Elem.Value; // Elem의 Value를 tmpRoom에 저장
        if (tmpRoom->bIsBossRoom) // 보스 방이 있다면
        {
            BossRoomExist = true; // 보스 방이 있다고 표시
            break; // 반복문 종료
        }
    }
    if (!BossRoomExist) // 보스 방이 없다면
    {
        for (auto& Elem : WMap) // WMap의 모든 요소에 대해 반복
        {
            FRoom* tmpRoom = &Elem.Value; // Elem의 Value를 tmpRoom에 저장
            if (tmpRoom->bIsEndRoom) // 마지막 방이라면
            {
                tmpRoom->bIsBossRoom = true; // 보스 방으로 설정
                break; // 반복문 종료
            }
        }
    }
    for (int i = -7; i < 8; i++)
    {
        for (int j = -7; j < 8; j++)
        {
            FIntPoint pointer(i, j);
            if (WMap.Contains(pointer))
            {
                FRoom* tmpRoom = WMap.Find(pointer);
                ARoomActor* RoomActor = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, tmpRoom->Location, FRotator::ZeroRotator);
                if (RoomActor)
                {
                    // 보스 방이거나 시작 방이라면 크기를 키움
                    if (tmpRoom->bIsBossRoom || tmpRoom->bIsStartRoom)
                    {
                        RoomActor->SetActorScale3D(FVector(1.3f));
                    }
                    else
                    {
                        RoomActor->SetActorScale3D(FVector(1.f));
                    }
                    RoomActor->SetRoomInfo(tmpRoom->Identity, tmpRoom->Location, tmpRoom->bIsEndRoom, tmpRoom->bIsBossRoom, tmpRoom->bIsStartRoom, tmpRoom->stretch);
                }
            }
        }
    }

}

EDirection AMyActorDuplicator::WhereIsBossRoom()
{
    float RandValue = FMath::FRand();
    if (RandValue < 0.25f)
    {
        return EDirection::UP;
    }
    else if (RandValue < 0.5f)
    {
        return EDirection::DOWN;
    }
    else if (RandValue < 0.75f)
    {
        return EDirection::RIGHT;
    }
    else
    {
        return EDirection::LEFT;
    }
}

// 실제 좌표 -> 배열 좌표 변환
FIntPoint AMyActorDuplicator::WorldToGrid(const FVector& WorldLocation)
{
    return FIntPoint(FMath::RoundToInt(WorldLocation.X / OffsetDistance.X), FMath::RoundToInt(WorldLocation.Y / OffsetDistance.Y));
}

// 배열 좌표 -> 실제 좌표 변환
FVector AMyActorDuplicator::GridToWorld(const FIntPoint& GridCoords)
{
    return FVector(GridCoords.X * OffsetDistance.X, GridCoords.Y * OffsetDistance.Y, 0.f);
}


// 랜덤 진행방향
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
