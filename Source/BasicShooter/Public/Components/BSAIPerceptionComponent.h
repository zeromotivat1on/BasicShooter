// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "BSAIPerceptionComponent.generated.h"

UCLASS()
class BASICSHOOTER_API UBSAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	AActor* GetClosestEnemy() const;
};
