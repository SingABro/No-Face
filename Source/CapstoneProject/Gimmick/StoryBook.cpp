// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/StoryBook.h"
#include "Components/BoxComponent.h"
//#include "Materials/MaterialInterface.h"

AStoryBook::AStoryBook()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetupAttachment(BoxComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	OnBeginCursorOver.AddDynamic(this, &AStoryBook::OnHighlighting);
	OnEndCursorOver.AddDynamic(this, &AStoryBook::OnEndHighlighting);
	OnClicked.AddDynamic(this, &AStoryBook::OnClick);
}

void AStoryBook::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStoryBook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStoryBook::OnHighlighting(AActor* TouchedActor)
{
	UE_LOG(LogTemp, Display, TEXT("Hi"));

	MeshComp->SetOverlayMaterial(HighlightMaterial);
}

void AStoryBook::OnEndHighlighting(AActor* TouchedActor)
{
	MeshComp->SetOverlayMaterial(nullptr);
}

void AStoryBook::OnClick(AActor* TouchedActor, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Display, TEXT("Click"));
}

