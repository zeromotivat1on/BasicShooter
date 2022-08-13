// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BSFireService.generated.h"

UCLASS()
class BASICSHOOTER_API UBSFireService : public UBTService
{
	GENERATED_BODY()

public:
	UBSFireService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
