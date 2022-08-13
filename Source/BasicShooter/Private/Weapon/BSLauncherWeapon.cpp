// Basic Shooter Game. All rights reserved

#include "Weapon/BSLauncherWeapon.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/BSProjectile.h"

void ABSLauncherWeapon::StartFire()
{
	PerformShot();
}

void ABSLauncherWeapon::PerformShot()
{
	if (!GetWorld()) return;

	if (IsAmmoEmpty())
	{
		// Almost the same as PlaySoundAtLocation but returns a pointer to AudioComponent that was spawned
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	PerformHit(HitResult, TraceStart, TraceEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	ABSProjectile* Projectile = GetWorld()->SpawnActorDeferred<ABSProjectile>(ProjectileClass, SpawnTransform);
	if (Projectile)
	{
		Projectile->SetOwner(GetOwner());
		Projectile->SetShotDirection(Direction);
		Projectile->FinishSpawning(SpawnTransform);
	}
	// TODO: set projectile params

	DecreaseAmmo();
	SpawnMuzzleFX();

	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
}
