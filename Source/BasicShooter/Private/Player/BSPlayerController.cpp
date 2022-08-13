// Basic Shooter Game. All rights reserved

#include "Player/BSPlayerController.h"
#include "BSGameModeBase.h"
#include "BSGameInstance.h"
#include "Player/BSPlayerCharacter.h"
#include "Components/BSWeaponComponent.h"
#include "Components/BSRespawnComponent.h"

ABSPlayerController::ABSPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<UBSRespawnComponent>("RespawnComponent");
}

void ABSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SubscribeOnMatchStateChanged();
}

void ABSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent) return;

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ABSPlayerController::OnPauseGame);
	InputComponent->BindAction("Mute", IE_Pressed, this, &ABSPlayerController::OnMuteSound);
}

void ABSPlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode() || !GetCharacter()) return;
	GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ABSPlayerController::OnMuteSound()
{
	if (!GetWorld()) return;

	const auto BSGameInstance = GetWorld()->GetGameInstance<UBSGameInstance>();
	if (!BSGameInstance) return;

	BSGameInstance->ToggleVolume();
}

void ABSPlayerController::SubscribeOnMatchStateChanged()
{
	if (GetWorld())
	{
		const auto GameMode = Cast<ABSGameModeBase>(GetWorld()->GetAuthGameMode());
		if (!GameMode) return;

		GameMode->OnMatchStateChanged.AddUObject(this, &ABSPlayerController::OnMatchStateChanged);
	}
}

void ABSPlayerController::OnMatchStateChanged(EBSMatchState State)
{
	if (State == EBSMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false; //
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true; //
	}
}
