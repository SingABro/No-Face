// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HighlightInterface.h"
#include "StoryBook.generated.h"

UCLASS()
class CAPSTONEPROJECT_API AStoryBook : public AActor, public IHighlightInterface
{
	GENERATED_BODY()
	
public:	
	AStoryBook();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void HighlightActor() override;
	virtual void EndHighlightActor() override;

	UFUNCTION()
	void OnHighlighting(AActor* TouchedActor);

	UFUNCTION()
	void OnEndHighlighting(AActor* TouchedActor);

	UFUNCTION()
	void OnClick(AActor* TouchedActor, FKey ButtonPressed);

private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class UBoxComponent> BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<class UMaterialInstance> HighlightMaterial;
};