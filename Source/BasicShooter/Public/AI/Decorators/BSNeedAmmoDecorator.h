// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BSNeedAmmoDecorator.generated.h"

class ABSBaseWeapon;

UCLASS()
class BASICSHOOTER_API UBSNeedAmmoDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBSNeedAmmoDecorator();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<ABSBaseWeapon> WeaponType;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
