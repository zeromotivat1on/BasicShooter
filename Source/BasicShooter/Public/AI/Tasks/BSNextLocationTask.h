// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BSNextLocationTask.generated.h"

UCLASS()
class BASICSHOOTER_API UBSNextLocationTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBSNextLocationTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Radius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector AimLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool SelfCenter = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (EditCondition = "!SelfCenter"))
	FBlackboardKeySelector CenterActorKey;
};
