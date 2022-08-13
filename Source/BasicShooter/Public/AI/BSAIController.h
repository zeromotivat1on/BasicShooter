// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BSAIController.generated.h"

class UBSAIPerceptionComponent;
class UBSRespawnComponent;

UCLASS()
class BASICSHOOTER_API ABSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABSAIController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBSAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBSRespawnComponent* RespawnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName FocusOnKeyName = "EnemyActor";

	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

private:
	AActor* GetFocusOnActor() const;
};
