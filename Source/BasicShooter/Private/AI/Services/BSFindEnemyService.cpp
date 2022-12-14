// Basic Shooter Game. All rights reserved

#include "AI/Services/BSFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Components/BSAIPerceptionComponent.h"

UBSFindEnemyService::UBSFindEnemyService()
{
	NodeName = "Find Enemy";
}

void UBSFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionComponent = Controller->FindComponentByClass<UBSAIPerceptionComponent>();
		if (PerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy()); //
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
