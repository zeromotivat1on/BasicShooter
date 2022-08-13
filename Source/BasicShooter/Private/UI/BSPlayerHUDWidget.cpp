// Basic Shooter Game. All rights reserved

#include "UI/BSPlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/ActorComponent.h"
#include "BSUtils.h"
#include "Components/BSHealthComponent.h"
#include "Components/BSWeaponComponent.h"

void UBSPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &ThisClass::OnNewPawn);

		/**
		 * As NativeOnInitialized() function get called after OnPosses(),
		 * so we should call OnNewPawn() mannualy at the first time
		 */
		OnNewPawn(GetOwningPlayerPawn());
	}
}

void UBSPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
	const auto HealthComponent = BSUtils::GetBSPlayerComponent<UBSHealthComponent>(NewPawn);
	if (HealthComponent && !HealthComponent->OnHealthChanged.IsBoundToObject(this))
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &UBSPlayerHUDWidget::OnHealthChanged); //
	}

	UpdateHealthBar();
}

void UBSPlayerHUDWidget::OnHealthChanged(float NewHealth, float HealthDelta)
{
	if (HealthDelta < 0.0f)
	{
		OnTakeDamage();

		if (!IsAnimationPlaying(DamageAnimation))
		{
			PlayAnimation(DamageAnimation); //
		}
	}

	UpdateHealthBar();
}

void UBSPlayerHUDWidget::UpdateHealthBar()
{
	if (!HealthProgressBar) return;

	HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? HighHealthColor : LowHealthColor);
}

float UBSPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = BSUtils::GetBSPlayerComponent<UBSHealthComponent>(GetOwningPlayerPawn());
	if (!HealthComponent) return false;

	return HealthComponent->GetHealthPercent();
}

bool UBSPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const
{
	const auto WeaponComponent = BSUtils::GetBSPlayerComponent<UBSWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponUIData(WeaponUIData);
}

bool UBSPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = BSUtils::GetBSPlayerComponent<UBSWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UBSPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = BSUtils::GetBSPlayerComponent<UBSHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

bool UBSPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

FString UBSPlayerHUDWidget::FormatBullets(int BulletsNum) const
{
	const int32 MaxLen = 3;
	const TCHAR PrefixSymbol = '0';

	FString BulletString = FString::FromInt(BulletsNum);
	const int32 SymbolsNumToAdd = MaxLen - BulletString.Len();

	if (SymbolsNumToAdd > 0)
	{
		BulletString = FString::ChrN(SymbolsNumToAdd, PrefixSymbol).Append(BulletString); //
	}

	return BulletString;
}
