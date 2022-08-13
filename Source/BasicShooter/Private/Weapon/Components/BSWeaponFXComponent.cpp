// Basic Shooter Game. All rights reserved

#include "Weapon/Components/BSWeaponFXComponent.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UBSWeaponFXComponent::UBSWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBSWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
	FImpactData ImpactData = DefaultImpactData;
	if (Hit.PhysMaterial.IsValid())
	{
		UPhysicalMaterial* PhysMaterial = Hit.PhysMaterial.Get();
		if (ImpactDataMap.Contains(PhysMaterial))
		{
			ImpactData = ImpactDataMap[PhysMaterial]; //
		}
	}

	// niagara
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), //
		ImpactData.NiagaraEffect,							   //
		Hit.ImpactPoint,									   //
		Hit.ImpactNormal.Rotation());

	// decal
	UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), //
		ImpactData.DecalData.Material,													 //
		ImpactData.DecalData.Size,														 //
		Hit.ImpactPoint,																 //
		Hit.ImpactNormal.Rotation());

	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime); //
	}

	// sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.Sound, Hit.ImpactPoint);
}
