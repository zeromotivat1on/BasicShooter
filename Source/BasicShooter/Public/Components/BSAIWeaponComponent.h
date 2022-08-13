// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/BSWeaponComponent.h"
#include "BSAIWeaponComponent.generated.h"

UCLASS()
class BASICSHOOTER_API UBSAIWeaponComponent : public UBSWeaponComponent
{
	GENERATED_BODY()
	
public:
	virtual void StartFire() override;
	virtual void NextWeapon() override;
};
