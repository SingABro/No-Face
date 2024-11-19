// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/NextMapDoor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ANextMapDoor::ANextMapDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComp;
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ANextMapDoor::OnBeginOverlap);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetupAttachment(BoxComp);
}

void ANextMapDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANextMapDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANextMapDoor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("Test")));
}

