// Basic Shooter Game. All rights reserved

#include "UI/BSSpectatorWidget.h"
#include "Components/BSRespawnComponent.h"

bool UBSSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	const auto RespawnComponent = GetOwningPlayer()->FindComponentByClass<UBSRespawnComponent>();
	if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

	CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;
}
