// Basic Shooter Game. All rights reserved

#include "AI/Services/BSFireService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Components/BSWeaponComponent.h"
#include "BSUtils.h"

UBSFireService::UBSFireService()
{
	NodeName = "Fire";
}

void UBSFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	const bool HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if (Controller)
	{
		const auto WeaponComponent = BSUtils::GetBSPlayerComponent<UBSWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent)
		{
			HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire(); //
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
