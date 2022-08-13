// Basic Shooter Game. All rights reserved

#include "Components/BSAIWeaponComponent.h"
#include "Weapon/BSBaseWeapon.h"

void UBSAIWeaponComponent::StartFire()
{
	if (!CanFire()) return;

	if (CurrentWeapon->IsAmmoEmpty())
	{
		NextWeapon(); //
	}
	else
	{
		CurrentWeapon->StartFire(); //
	}
}

void UBSAIWeaponComponent::NextWeapon()
{
	if (!CanFire()) return;

	int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	while (NextIndex != CurrentWeaponIndex)
	{
		if (!Weapons[NextIndex]->IsAmmoEmpty()) break;
		NextIndex = (NextIndex + 1) % Weapons.Num();
	}

	if (CurrentWeaponIndex != NextIndex)
	{
		CurrentWeaponIndex = NextIndex;
		EquipWeapon(CurrentWeaponIndex);
	}
}