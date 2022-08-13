// Basic Shooter Game. All rights reserved

#include "Pickups/BSHealthPickup.h"
#include "Components/BSHealthComponent.h"
#include "BSUtils.h"

bool ABSHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = BSUtils::GetBSPlayerComponent<UBSHealthComponent>(PlayerPawn);
	if (!HealthComponent) return false;

	return HealthComponent->TryAddHealth(HealthAmount);
}
