#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomActor.generated.h"

UCLASS()
class CAPSTONEPROJECT_API ARoomActor : public AActor
{
    GENERATED_BODY()

public:
    ARoomActor();

public:
    void SetRoomInfo(int32 InIdentity, const FVector& InLocation, bool InbIsEndRoom);

private:
    UPROPERTY(EditAnywhere, Category = "Room")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    UPROPERTY(EditAnywhere, Category = "Room")
    int32 Identity;  // 방의 정체성 (비트 플래그)

    UPROPERTY(EditAnywhere, Category = "Room")
    FVector Location;  // 방의 월드 좌표

    UPROPERTY(EditAnywhere, Category = "Room")
    bool bIsEndRoom;  // 마지막 방 여부
};
