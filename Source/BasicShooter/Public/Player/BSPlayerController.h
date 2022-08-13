// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BSCoreTypes.h"
#include "BSPlayerController.generated.h"

class UBSRespawnComponent;

UCLASS()
class BASICSHOOTER_API ABSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABSPlayerController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBSRespawnComponent* RespawnComponent;

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	void OnPauseGame();
	void OnMuteSound();

	void SubscribeOnMatchStateChanged();
	void OnMatchStateChanged(EBSMatchState State);
};
