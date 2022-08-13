// Basic Shooter Game. All rights reserved

#include "AI/BSAIController.h"
#include "AI/BSAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BSRespawnComponent.h"
#include "Components/BSAIPerceptionComponent.h"

ABSAIController::ABSAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UBSAIPerceptionComponent>("BSAIPerceptionComponent");
	SetPerceptionComponent(*AIPerceptionComponent);

	RespawnComponent = CreateDefaultSubobject<UBSRespawnComponent>("RespawnComponent");

	bWantsPlayerState = true;
}

void ABSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto BSAICharacter = Cast<ABSAICharacter>(InPawn);
	if (BSAICharacter)
	{
		 RunBehaviorTree(BSAICharacter->BehaviorTreeAsset); //
	}
}

void ABSAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

AActor* ABSAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
