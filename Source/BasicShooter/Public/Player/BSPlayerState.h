// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BSPlayerState.generated.h"

UCLASS()
class BASICSHOOTER_API ABSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void SetTeamID(int32 ID) { TeamID = ID; }
	int32 GetTeamID() const { return TeamID; }

	void SetTeamColor(const FLinearColor& Color) { TeamColor = Color; }
	FLinearColor GetTeamColor() const { return TeamColor; }

	void AddKill() { ++KillsNum; }
	int32 GetKillsNum() const { return KillsNum; }

	void AddDeath() { ++DeathsNum; }
	int32 GetDeathsNum() const { return DeathsNum; }

	void LogInfo();

private:
	int32 TeamID;
	FLinearColor TeamColor;

	int32 KillsNum = 0;
	int32 DeathsNum = 0;
};
