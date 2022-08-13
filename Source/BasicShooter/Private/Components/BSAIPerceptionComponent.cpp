// Basic Shooter Game. All rights reserved

#include "Components/BSAIPerceptionComponent.h"
#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "BSUtils.h"
#include "Components/BSHealthComponent.h"

AActor* UBSAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PerceiveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);

	// TODO: FIX: If bot sees player from his team, while he is shot in back, he wont rotate
	if (PerceiveActors.Num() == 0)
	{
		GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PerceiveActors);
		if (PerceiveActors.Num() == 0) return nullptr;
	}

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;
	for (const auto& PerceiveActor : PerceiveActors)
	{
		const auto HealthComponent = PerceiveActor->FindComponentByClass<UBSHealthComponent>();

		const auto PerceivePawn = Cast<APawn>(PerceiveActor);
		const bool AreEnemies = PerceivePawn && BSUtils::AreEnemies(Controller, PerceivePawn->GetController());

		if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
		{
			const double CurrentDistance = (PerceiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PerceiveActor;
			}
		}
	}

	return BestPawn;
}
