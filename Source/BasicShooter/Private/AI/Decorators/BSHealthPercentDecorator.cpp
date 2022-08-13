// Basic Shooter Game. All rights reserved

#include "AI/Decorators/BSHealthPercentDecorator.h"
#include "AIController.h"
#include "Components/BSHealthComponent.h"
#include "BSUtils.h"

UBSHealthPercentDecorator::UBSHealthPercentDecorator() 
{
	NodeName = "Health Percent";
}

bool UBSHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	const auto HealthComponent = BSUtils::GetBSPlayerComponent<UBSHealthComponent>(Controller->GetPawn());
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	return HealthComponent->GetHealthPercent() <= HealthPercent;
}
