// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BSBasePickup.h"
#include "BSAmmoPickup.generated.h"

class ABSBaseWeapon;

UCLASS()
class BASICSHOOTER_API ABSAmmoPickup : public ABSBasePickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	int32 ClipsAmount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	TSubclassOf<ABSBaseWeapon> WeaponType;

private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
