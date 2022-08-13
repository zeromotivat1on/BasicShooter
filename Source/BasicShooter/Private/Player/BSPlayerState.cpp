// Basic Shooter Game. All rights reserved

#include "Player/BSPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogBSPlayerState, All, All);

void ABSPlayerState::LogInfo()
{
	UE_LOG(LogBSPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}
