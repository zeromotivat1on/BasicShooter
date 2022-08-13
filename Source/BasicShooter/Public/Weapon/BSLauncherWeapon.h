// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BSBaseWeapon.h"
#include "BSLauncherWeapon.generated.h"

class ABSProjectile;
class USoundCue;

UCLASS()
class BASICSHOOTER_API ABSLauncherWeapon : public ABSBaseWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ABSProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* NoAmmoSound;

	virtual void PerformShot() override;
};
