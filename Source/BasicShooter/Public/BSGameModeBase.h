// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BSCoreTypes.h"
#include "BSGameModeBase.generated.h"

class AAIController;

UCLASS()
class BASICSHOOTER_API ABSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABSGameModeBase();

	FOnMatchStateChangedSignature OnMatchStateChanged;

	virtual void StartPlay() override;

	/**
	 * As we spawn controllers and then ask game mode base to spawn characters for them,
	 * it spawns default pawn, which is BSBaseCharacter, but we need BSAICharacter,
	 * so we should override this method to get pawn we need.
	 */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	void Killed(AController* Killer, AController* Victim);

	int32 GetCurrentRoundNum() const { return CurrentRound; }
	FGameData GetGameData() const { return GameData; }
	int32 GetRoundSecondsRemaining() const { return RoundCountDown; }

	void RespawnRequest(AController* Controller);

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FGameData GameData;

private:
	EBSMatchState MatchState = EBSMatchState::WaitingToStart;

	int32 CurrentRound = 1;
	int32 RoundCountDown = 0;
	FTimerHandle GameRoundTimerHandle;

	void SpawnBots();
	void StartRound();
	void GameTimerUpdate();

	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);

	void CreateTeamsInfo();
	FLinearColor DetermineColorByTeamID(int32 TeamID) const;
	void SetPlayerColor(AController* Controller);

	void LogPlayerInfo();

	void StartRespawn(AController* Controller);

	void GameOver();

	void SetMathcState(EBSMatchState State);

	void StopAllFire();
};
