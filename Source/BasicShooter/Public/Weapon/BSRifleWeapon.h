// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BSBaseWeapon.h"
#include "BSRifleWeapon.generated.h"

class UBSWeaponFXComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UAudioComponent;

UCLASS()
class BASICSHOOTER_API ABSRifleWeapon : public ABSBaseWeapon
{
	GENERATED_BODY()

public:
	ABSRifleWeapon();

	virtual void StartFire() override;
	virtual void StopFire() override;

	virtual void Zoom(bool Enabled) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TimeBetweenShots = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float BulletSpread = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FString TraceTargetName = "TraceTarget";

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	UBSWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float FOVZoomAngle = 50.0f;

	virtual void BeginPlay() override;

	virtual void PerformShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:
	FTimerHandle ShotTimerHandle;
	float FOVDefaultAngle = 90.0f;

	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;

	UPROPERTY()
	UAudioComponent* FireAudioComponent;

	void PerformDamage(const FHitResult& HitResult);

	void InitFX();
	void InitMuzzleFX();
	void SetMuzzleFXVisibility(bool Visible);
	void InitFireSFX();
	void SetFireSFXVolume(bool IsActive);
	void SetFXActive(bool IsActive);

	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

	AController* GetController() const;
};
