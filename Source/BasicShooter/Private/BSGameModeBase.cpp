// Basic Shooter Game. All rights reserved

#include "BSGameModeBase.h"
#include "EngineUtils.h"
#include "AIController.h"
#include "UI/BSGameHUD.h"
#include "Player/BSPlayerState.h"
#include "Player/BSBaseCharacter.h"
#include "Player/BSPlayerController.h"
#include "Components/BSWeaponComponent.h"
#include "Components/BSRespawnComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBSGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10;

ABSGameModeBase::ABSGameModeBase()
{
	DefaultPawnClass = ABSBaseCharacter::StaticClass();
	PlayerControllerClass = ABSPlayerController::StaticClass();
	HUDClass = ABSGameHUD::StaticClass();
	PlayerStateClass = ABSPlayerState::StaticClass();
}

void ABSGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnBots();
	CreateTeamsInfo();

	CurrentRound = 1;
	StartRound();

	SetMathcState(EBSMatchState::InProgress);
}

UClass* ABSGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass; //
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ABSGameModeBase::SpawnBots()
{
	if (!GetWorld()) return;

	for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const auto BSAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
		RestartPlayer(BSAIController);
	}
}

void ABSGameModeBase::StartRound()
{
	RoundCountDown = GameData.RoundTime;
	GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ThisClass::GameTimerUpdate, 1.0f, true);
}

void ABSGameModeBase::GameTimerUpdate()
{
	//UE_LOG(LogBSGameModeBase, Display, TEXT("Time: %i, Round: %i / %i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

	/* Possible implementation using timer rate */
	// const auto TimerRate = GetWorldTimerManager().GetTimerRate(GameRoundTimerHandle);
	// RoundCountDown -= TimerRate;

	if (--RoundCountDown == 0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

		if (CurrentRound + 1 <= GameData.RoundsNum)
		{
			++CurrentRound;
			ResetPlayers();
			StartRound();
		}
		else
		{
			GameOver();
		}
	}
}

void ABSGameModeBase::ResetPlayers()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		ResetOnePlayer(It->Get()); //
	}
}

void ABSGameModeBase::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset(); //
	}

	RestartPlayer(Controller);
	SetPlayerColor(Controller);
}

void ABSGameModeBase::CreateTeamsInfo()
{
	if (!GetWorld()) return;

	int32 TeamID = 1;
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<ABSPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;

		PlayerState->SetTeamID(TeamID);
		PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
		PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
		SetPlayerColor(Controller);

		TeamID = TeamID == 1 ? 2 : 1;
	}
}

FLinearColor ABSGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
	if (TeamID - 1 < GameData.TeamColors.Num())
	{
		return GameData.TeamColors[TeamID - 1]; //
	}

	UE_LOG(LogBSGameModeBase, Warning, TEXT("No color for team id: %i, setting to default: %s"), TeamID, CurrentRound,
		*GameData.DefaultTeamColor.ToString());

	return GameData.DefaultTeamColor;
}

void ABSGameModeBase::SetPlayerColor(AController* Controller)
{
	if (!Controller) return;

	const auto Character = Cast<ABSBaseCharacter>(Controller->GetPawn());
	if (!Character) return;

	const auto PlayerState = Cast<ABSPlayerState>(Controller->PlayerState);
	if (!PlayerState) return;

	Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ABSGameModeBase::Killed(AController* Killer, AController* Victim)
{
	const auto KillerPlayerState = Killer ? Cast<ABSPlayerState>(Killer->PlayerState) : nullptr;
	const auto VictimPlayerState = Victim ? Cast<ABSPlayerState>(Victim->PlayerState) : nullptr;

	if (KillerPlayerState)
	{
		KillerPlayerState->AddKill(); //
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddDeath(); //
	}

	StartRespawn(Victim);
}

void ABSGameModeBase::LogPlayerInfo()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<ABSPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;

		PlayerState->LogInfo();
	}
}

void ABSGameModeBase::RespawnRequest(AController* Controller)
{
	ResetOnePlayer(Controller);
}

bool ABSGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const bool PauseSet = Super::SetPause(PC, CanUnpauseDelegate);

	if (PauseSet)
	{
		StopAllFire();
		SetMathcState(EBSMatchState::Pause); //
	}

	return PauseSet;
}

bool ABSGameModeBase::ClearPause()
{
	const bool PauseCleared = Super::ClearPause();

	if (PauseCleared)
	{
		SetMathcState(EBSMatchState::InProgress);
	}

	return PauseCleared;
}

void ABSGameModeBase::StartRespawn(AController* Controller)
{
	const bool RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
	if (!RespawnAvailable) return;

	const auto RespawnComponent = Controller->FindComponentByClass<UBSRespawnComponent>();
	if (!RespawnComponent) return;

	RespawnComponent->Respawn(GameData.RespawnTime);
}

void ABSGameModeBase::GameOver()
{
	//LogPlayerInfo();

	for (const auto& Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}

	SetMathcState(EBSMatchState::GameOver);
}

void ABSGameModeBase::SetMathcState(EBSMatchState State)
{
	if (MatchState == State) return;

	MatchState = State;
	OnMatchStateChanged.Broadcast(State);
}

void ABSGameModeBase::StopAllFire() 
{
	for (const auto& Pawn : TActorRange<APawn>(GetWorld())) 
	{
		const auto WeaponComponent = Pawn->FindComponentByClass<UBSWeaponComponent>();
		if (!WeaponComponent) continue;

		WeaponComponent->StopFire();
		WeaponComponent->Zoom(false);
	}
}
