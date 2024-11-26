// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"

ACGameModeBase::ACGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/No-Face/Blueprints/Character/BP_CharacterBase.BP_CharacterBase_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/CapstoneProject.CPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> WinScreenClassRef(TEXT("/Game/No-Face/UI/WBP_Ending.WBP_Ending_C"));
	if (WinScreenClassRef.Class)
	{
		WinScreenClass = WinScreenClassRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SoundRef(TEXT("/Script/Engine.SoundWave'/Game/No-Face/SkillSound/BGM/0-top-battle-game-bgm-264625.0-top-battle-game-bgm-264625'"));
	if (SoundRef.Object)
	{
		BGM = SoundRef.Object;
	}

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
}

void ACGameModeBase::GameEnd()
{
	UUserWidget* WinScreen = CreateWidget(GetWorld(), WinScreenClass);
	if (WinScreen)
	{
		WinScreen->AddToViewport();
	}

	/*FTimerHandle UITimer;
	GetWorld()->GetTimerManager().SetTimer(UITimer, [&]()
		{
			WinScreen->RemoveFromViewport();
		}, 7.f, false);*/
}

void ACGameModeBase::BeginPlay()
{
	Super::BeginPlay();
		
	AudioComponent->SetSound(BGM);
	AudioComponent->Play();
}
