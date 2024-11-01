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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
    TObjectPtr<UStaticMeshComponent> MeshComponent;
};
