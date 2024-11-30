#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorDuplicator.generated.h"


// 방의 방향을 나타내는 열거형
UENUM()
enum class EDirection : uint8
{
    UP = 1,
    RIGHT = 2,
    DOWN = 4,
    LEFT = 8
};

// Room 구조체
USTRUCT()
struct FRoom
{
    GENERATED_BODY()

    int32 Identity;  // 방의 정체성 (비트 플래그)
    int stretch; // 몇 번 뻗어나갔는지
    FVector Location;  // 방의 월드 좌표
    bool bIsEndRoom;  // 마지막 방 여부
    bool bIsBossRoom; // 보스 방 여부
	bool bIsStartRoom; // 시작 방 여부   

    FRoom() : Identity(0), Location(FVector::ZeroVector), bIsEndRoom(false), bIsBossRoom(false), bIsStartRoom(false), stretch(0) {}
};

UCLASS()
class CAPSTONEPROJECT_API AMyActorDuplicator : public AActor
{
    GENERATED_BODY()

public:
    AMyActorDuplicator();

protected:
    virtual void BeginPlay() override;

private:
    // 기존 Rooms 배열
    TArray<FRoom> Rooms;

    // 2D 월드 그리드 맵 (FIntPoint 좌표와 FRoom의 맵핑)
    TMap<FIntPoint, FRoom> WorldMap;

    // Static Mesh와 방의 거리 오프셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duplication", meta = (AllowPrivateAccess = "true"))
    FVector OffsetDistance = FVector(3000.f, 3000.f, 0.f);

    // 생성할 방의 최대 깊이
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duplication", meta = (AllowPrivateAccess = "true"))
    int32 MaxDepth = 7;

    // Room Actor 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duplication", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class ARoomActor> RoomActorClass;

    // 랜덤 방향 선택 함수
    EDirection GetRandomDirection(EDirection CurrentDir);

    // 방 생성 함수 (재귀 호출)
    void CreateRooms(const FIntPoint& CurrentCoords, int32 Depth, EDirection CurrentDir, EDirection InitDir, EDirection BossDir);

    // 맵 생성 함수
    void BuildActualStage(const TMap<FIntPoint, FRoom> WorldMap);

    // 보스 방 방위 함수
    EDirection WhereIsBossRoom();

    // 월드 좌표에서 FIntPoint 좌표로 변환
    FIntPoint WorldToGrid(const FVector& WorldLocation);

    // FIntPoint 좌표에서 월드 좌표로 변환
    FVector GridToWorld(const FIntPoint& GridCoords);

	// 미니맵 오프셋
	FVector MinimapOffset = FVector(10000, 10000, 8000);

    UPROPERTY(EditAnywhere, Category = "Map")
    TSubclassOf<class UWorldmapWidget> WorldmapWidgetClass;
};
