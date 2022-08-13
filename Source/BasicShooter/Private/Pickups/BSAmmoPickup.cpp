// Basic Shooter Game. All rights reserved

#include "Pickups/BSAmmoPickup.h"
#include "Components/BSWeaponComponent.h"
#include "Components/BSHealthComponent.h"
#include "BSUtils.h"

bool ABSAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = BSUtils::GetBSPlayerComponent<UBSHealthComponent>(PlayerPawn);
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	const auto WeaponComponent = BSUtils::GetBSPlayerComponent<UBSWeaponComponent>(PlayerPawn);
	if (!WeaponComponent) return false;

	return WeaponComponent->TryAddAmmo(WeaponType, ClipsAmount);
}
