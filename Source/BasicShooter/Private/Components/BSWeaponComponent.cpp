// Basic Shooter Game. All rights reserved

#include "Components/BSWeaponComponent.h"
#include "Weapon/BSBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/BSEquipFinishedAnimNotify.h"
#include "Animations/BSReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

UBSWeaponComponent::UBSWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBSWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Character can hold only %d weapons"), WeaponNum);

	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void UBSWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (const auto& Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}

	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

void UBSWeaponComponent::SpawnWeapons()
{
	if (!GetWorld()) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	for (const auto& OneWeaponData : WeaponData)
	{
		ABSBaseWeapon* const Weapon = GetWorld()->SpawnActor<ABSBaseWeapon>(OneWeaponData.WeaponClass);
		if (!Weapon) continue;

		Weapon->SetOwner(Character);
		Weapon->OnClipEmpty.AddUObject(this, &UBSWeaponComponent::OnClipEmpty);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void UBSWeaponComponent::AttachWeaponToSocket(ABSBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;

	FAttachmentTransformRules AttachmentRules(WeaponAttachmentRule, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UBSWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index: %d"), WeaponIndex);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		CurrentWeapon->Zoom(false);

		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}

	CurrentWeapon = Weapons[WeaponIndex];
	// CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
	const auto CurrentWeaponData = WeaponData.FindByPredicate(
		[&](const FWeaponData& WeaponData)
		{
			return WeaponData.WeaponClass == CurrentWeapon->GetClass(); //
		});

	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

void UBSWeaponComponent::StartFire()
{
	if (!CanFire()) return;
	CurrentWeapon->StartFire();
}

void UBSWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void UBSWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void UBSWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(AnimMontage);
}

void UBSWeaponComponent::InitAnimations()
{
	const auto EquipFinishedNotify = AnimUtils::FindFirstNotifyByClass<UBSEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &UBSWeaponComponent::OnEquipFinished); //
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}

	for (const auto& OneWeaponData : WeaponData)
	{
		const auto ReloadFinishedNotify = AnimUtils::FindFirstNotifyByClass<UBSReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify)
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
			checkNoEntry();
		}

		ReloadFinishedNotify->OnNotified.AddUObject(this, &UBSWeaponComponent::OnReloadFinished);
	}
}

void UBSWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	EquipAnimInProgress = false;
}

void UBSWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	ReloadAnimInProgress = false;
}

bool UBSWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UBSWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UBSWeaponComponent::CanReload() const
{
	return CurrentWeapon			//
		   && !EquipAnimInProgress	//
		   && !ReloadAnimInProgress //
		   && CurrentWeapon->CanReload();
}

void UBSWeaponComponent::Reload()
{
	ChangeClip();
}

bool UBSWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const
{
	if (CurrentWeapon)
	{
		WeaponUIData = CurrentWeapon->GetUIData();
		return true;
	}

	return false;
}

bool UBSWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}

	return false;
}

bool UBSWeaponComponent::TryAddAmmo(TSubclassOf<ABSBaseWeapon> WeaponType, int32 ClipsAmount)
{
	for (const auto& Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryAddAmmo(ClipsAmount); //
		}
	}

	return false;
}

bool UBSWeaponComponent::NeedAmmo(TSubclassOf<ABSBaseWeapon> WeaponType)
{
	for (const auto& Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return !Weapon->IsAmmoFull(); //
		}
	}

	return false;
}

void UBSWeaponComponent::Zoom(bool Enabled)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Zoom(Enabled); //
	}
}

void UBSWeaponComponent::OnClipEmpty(ABSBaseWeapon* AmmoEmptyWeapon)
{
	if (!AmmoEmptyWeapon) return;

	if (CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip(); //
	}
	else
	{
		for (const auto& Weapon : Weapons)
		{
			if (Weapon == AmmoEmptyWeapon)
			{
				Weapon->ChangeClip(); //
			}
		}
	}
}

void UBSWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;

	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();

	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}