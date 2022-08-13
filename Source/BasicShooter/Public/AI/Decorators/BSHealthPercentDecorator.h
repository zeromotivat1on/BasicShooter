// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BSHealthPercentDecorator.generated.h"

UCLASS()
class BASICSHOOTER_API UBSHealthPercentDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBSHealthPercentDecorator();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float HealthPercent = 0.6f;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
