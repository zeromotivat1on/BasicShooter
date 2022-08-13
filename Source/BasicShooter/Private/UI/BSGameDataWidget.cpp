// Basic Shooter Game. All rights reserved

#include "UI/BSGameDataWidget.h"
#include "BSGameModeBase.h"
#include "Player/BSPlayerState.h"

int32 UBSGameDataWidget::GetKillsNum() const
{
	const auto PlayerState = GetBSPlayerState();
	return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 UBSGameDataWidget::GetDeathsNum() const
{
	const auto PlayerState = GetBSPlayerState();
	return PlayerState ? PlayerState->GetDeathsNum() : 0;
}

int32 UBSGameDataWidget::GetCurrentRoundNum() const
{
	const auto GameMode = GetBSGameMode();
	return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 UBSGameDataWidget::GetTotalRoundsNum() const
{
	const auto GameMode = GetBSGameMode();
	return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

int32 UBSGameDataWidget::GetRoundSecondsRemaining() const
{
	const auto GameMode = GetBSGameMode();
	return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

ABSGameModeBase* UBSGameDataWidget::GetBSGameMode() const
{
	return GetWorld() ? Cast<ABSGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ABSPlayerState* UBSGameDataWidget::GetBSPlayerState() const
{
	return GetOwningPlayer() ? Cast<ABSPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
