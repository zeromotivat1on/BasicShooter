// Basic Shooter Game. All rights reserved

#include "Weapon/BSRifleWeapon.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Weapon/Components/BSWeaponFXComponent.h"

ABSRifleWeapon::ABSRifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<UBSWeaponFXComponent>("WeaponFXComponent");
}

void ABSRifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponFXComponent);
}

void ABSRifleWeapon::StartFire()
{
	InitFX();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ABSRifleWeapon::PerformShot, TimeBetweenShots, true);
	PerformShot();
}

void ABSRifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetFXActive(false);
}

void ABSRifleWeapon::Zoom(bool Enabled)
{
	const auto Controller = Cast<APlayerController>(GetController());
	if (!Controller || !Controller->PlayerCameraManager) return;

	if (Enabled) 
	{ 
		FOVDefaultAngle = Controller->PlayerCameraManager->GetFOVAngle();
	}

	Controller->PlayerCameraManager->SetFOV(Enabled ? FOVZoomAngle : FOVDefaultAngle);
}

void ABSRifleWeapon::PerformShot()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}

	const auto Player = GetOwner();
	if (!Player) return;

	FHitResult HitResult;
	PerformHit(HitResult, TraceStart, TraceEnd);

	FVector TraceFXEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		TraceFXEnd = HitResult.ImpactPoint;
		PerformDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);
	}

	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);

	DecreaseAmmo();
}

bool ABSRifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const float HalfAngleRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfAngleRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

	return true;
}

void ABSRifleWeapon::PerformDamage(const FHitResult& HitResult)
{
	AActor* const DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	FPointDamageEvent PointDamageEvent;
	PointDamageEvent.HitInfo = HitResult;
	DamagedActor->TakeDamage(DamageAmount, PointDamageEvent, GetController(), this);
}

void ABSRifleWeapon::InitFX() 
{
	InitMuzzleFX();
	InitFireSFX();
}

void ABSRifleWeapon::InitMuzzleFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX(); //
	}

	SetMuzzleFXVisibility(true);
}

void ABSRifleWeapon::SetMuzzleFXVisibility(bool Visible)
{
	if (!MuzzleFXComponent) return;

	MuzzleFXComponent->SetPaused(!Visible);
	MuzzleFXComponent->SetVisibility(Visible, true);
}

void ABSRifleWeapon::InitFireSFX() 
{
	if (!FireAudioComponent)
	{
		FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName); //
	}

	SetFireSFXVolume(true);
}

void ABSRifleWeapon::SetFireSFXVolume(bool IsActive) 
{
	if (!FireAudioComponent) return;
	FireAudioComponent->SetPaused(!IsActive);
}

void ABSRifleWeapon::SetFXActive(bool IsActive) 
{
	SetMuzzleFXVisibility(IsActive);
	SetFireSFXVolume(IsActive);
}

void ABSRifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd); //
	}
}

AController* ABSRifleWeapon::GetController() const
{
	APawn* const Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}