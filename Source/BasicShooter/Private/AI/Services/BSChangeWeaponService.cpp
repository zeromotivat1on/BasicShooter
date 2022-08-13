// Basic Shooter Game. All rights reserved

#include "AI/Services/BSChangeWeaponService.h"
#include "Components/BSWeaponComponent.h"
#include "AIController.h"
#include "BSUtils.h"

UBSChangeWeaponService::UBSChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void UBSChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (Controller)
	{
		const auto WeaponComponent = BSUtils::GetBSPlayerComponent<UBSWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
		{
			WeaponComponent->NextWeapon(); //
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
