#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorDuplicator.generated.h"


// ���� ����
UENUM()
enum class EDirection : uint8
{
    UP = 1,
    RIGHT = 2,
    DOWN = 4,
    LEFT = 8
};

// Room ����ü
USTRUCT()
struct FRoom
{
    GENERATED_BODY()

    int32 Identity;
    FVector Location;
    bool bIsEndRoom;

    FRoom() : Identity(0), Location(FVector::ZeroVector), bIsEndRoom(false) {}
};

UCLASS()
class CAPSTONEPROJECT_API AMyActorDuplicator : public AActor
{
    GENERATED_BODY()

public:
    AMyActorDuplicator();

protected:
    virtual void BeginPlay() override;

public:
    // Static Mesh�� ������ �гο��� ������ �� �ֵ��� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duplication")
    TObjectPtr<class UStaticMesh> StaticMeshToDuplicate;  // Static Mesh ����

    // ������ Static Mesh�� ��ġ�� �̵���Ű�� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duplication")
    FVector OffsetDistance = FVector(200.f, 200.f, 0.f);  // �⺻��: 200 ���� X, Y �̵�

    // �� ���� �˰������ �ִ� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duplication")
    int32 MaxDepth = 7;

    // ������ �� ���� Ŭ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duplication")
    TSubclassOf<class ARoomActor> RoomActorClass;

    UFUNCTION()
    void DuplicateRoomsWithScale(const FVector& Newcoordinate, float ScaleFactor);

private:
    // ���� ������ �迭
    TArray<FRoom> Rooms;

    // ���� ��������� �����ϴ� �Լ�
    void CreateRooms(FRoom& CurrentRoom, int32 Depth, EDirection CurrentDir);

    // ���� ������ ������ ����
    EDirection GetRandomDirection(EDirection CurrentDir);
};