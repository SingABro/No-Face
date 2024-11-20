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

        // 시작 방 설정
        FRoom StartRoom;
        StartRoom.Location = FVector3d(0, 0, 0);
        StartRoom.Identity = 0;
        // 시작 좌표
        FIntPoint StartCoords = FIntPoint(0, 0);

        WorldMap.Add(StartCoords, StartRoom);

        // 트리 형태의 방 생성
        CreateRooms(StartCoords, MaxDepth, EDirection::UP);
        CreateRooms(StartCoords, MaxDepth, EDirection::DOWN);
        CreateRooms(StartCoords, MaxDepth, EDirection::RIGHT);
        CreateRooms(StartCoords, MaxDepth, EDirection::LEFT);

        BuildActualStage(WorldMap);
    }
}

void AMyActorDuplicator::CreateRooms(const FIntPoint& CurrentCoords, int32 Depth, EDirection CurrentDir)
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
    if (WorldMap.Find(NewCoords) != NULL)
    { // 이미 월드에 존재하면
        FRoom tmpRoom;
        WorldMap.FindAndRemoveChecked(NewCoords); // 기존 월드에 같은 좌표의 맵을 불러오고 삭제
        tmpRoom.Identity |= NewRoom.Identity; // identity 갱신
        tmpRoom.Location = GridToWorld(NewCoords);
        WorldMap.Add(NewCoords, tmpRoom); // wolrd에 다시 저장
    }
    else
    {
        WorldMap.Add(NewCoords, NewRoom);
    }

    // 재귀 호출
    CreateRooms(NewCoords, Depth - 1, NewDir);
}


// 방 Actor 생성
void AMyActorDuplicator::BuildActualStage(TMap<FIntPoint, FRoom> WMap)
{
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
                    RoomActor->SetActorScale3D(FVector(1.0f));
                    RoomActor->SetRoomInfo(tmpRoom->Identity, tmpRoom->Location, tmpRoom->bIsEndRoom);
                }
            }
        }
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
