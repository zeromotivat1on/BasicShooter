// Basic Shooter Game. All rights reserved

#include "Components/BSRespawnComponent.h"
#include "BSGameModeBase.h"

UBSRespawnComponent::UBSRespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBSRespawnComponent::Respawn(int32 RespawnTime)
{
	if (!GetWorld()) return;

	RespawnCountDown = RespawnTime;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UBSRespawnComponent::RespawnTimerUpdate, 1.0f, true);
}

void UBSRespawnComponent::RespawnTimerUpdate() 
{
	if (--RespawnCountDown == 0) 
	{
		if (!GetWorld()) return;
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

		const auto GameMode = Cast<ABSGameModeBase>(GetWorld()->GetAuthGameMode());
		if (!GameMode) return;

		GameMode->RespawnRequest(Cast<AController>(GetOwner()));
	}
}

bool UBSRespawnComponent::IsRespawnInProgress() const
{
	return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}
