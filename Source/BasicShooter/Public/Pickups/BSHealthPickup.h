// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BSBasePickup.h"
#include "BSHealthPickup.generated.h"

UCLASS()
class BASICSHOOTER_API ABSHealthPickup : public ABSBasePickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utils", meta = (ClampMin = "1.0", ClampMax = "100.0"))
	float HealthAmount = 100.0f;

private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
