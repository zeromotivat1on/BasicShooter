// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/BSBaseCharacter.h"
#include "BSAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class BASICSHOOTER_API ABSAICharacter : public ABSBaseCharacter
{
	GENERATED_BODY()

public:
	ABSAICharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float HealthVisiblityDistance = 2500.0f;

	virtual void BeginPlay() override;

	virtual void OnDeath() override;
	virtual void OnHealthChanged(float NewHealth, float HealthDelta) override;

private:
	// TODO (Possible Optimization): Rewrite with timer
	// Now is using Tick function
	void UpdateHealthWidgetVisibility();
};
