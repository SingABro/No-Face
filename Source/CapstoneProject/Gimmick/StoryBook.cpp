// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/StoryBook.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/StoryBookWidget.h"

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
	
	DialogUI = CreateWidget<UStoryBookWidget>(GetWorld(), DialogUIClass);
	ensure(DialogUI);

	DialogUI->OnLastPage.AddUObject(this, &AStoryBook::OnClose);
}

void AStoryBook::HighlightActor()
{
	bStoryBook = true;
	MeshComp->SetOverlayMaterial(HighlightMaterial);
}

void AStoryBook::EndHighlightActor()
{
	bStoryBook = false;
	MeshComp->SetOverlayMaterial(nullptr);
}

void AStoryBook::OnHighlighting(AActor* TouchedActor)
{
	UE_LOG(LogTemp, Display, TEXT("Hi"));

	HighlightActor();
}

void AStoryBook::OnEndHighlighting(AActor* TouchedActor)
{
	EndHighlightActor();
}

void AStoryBook::OnClick(AActor* TouchedActor, FKey ButtonPressed)
{
	if (!DialogUI->IsInViewport())
	{
		DialogUI->AddToViewport();
	}
}

void AStoryBook::OnClose()
{
	DialogUI->RemoveFromViewport();
}

