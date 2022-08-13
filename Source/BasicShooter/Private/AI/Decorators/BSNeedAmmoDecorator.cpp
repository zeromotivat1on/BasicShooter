// Basic Shooter Game. All rights reserved

#include "AI/Decorators/BSNeedAmmoDecorator.h"
#include "AIController.h"
#include "Components/BSweaponComponent.h"
#include "BSUtils.h"

UBSNeedAmmoDecorator::UBSNeedAmmoDecorator()
{
	NodeName = "Need Ammo";
}

bool UBSNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	const auto WeaponComponent = BSUtils::GetBSPlayerComponent<UBSWeaponComponent>(Controller->GetPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->NeedAmmo(WeaponType);
}
