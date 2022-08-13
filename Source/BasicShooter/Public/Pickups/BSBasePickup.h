// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSBasePickup.generated.h"

class USphereComponent;
class USoundCue;

UCLASS()
class BASICSHOOTER_API ABSBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ABSBasePickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RespawnTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* PickupTakenSound;

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:	
	virtual void Tick(float DeltaTime) override;

	bool CouldBeTaken() const;

private:
	UPROPERTY()
	TArray<APawn*> OverlappingPawns;

	FTimerHandle RespawnTimerHandle;

	float RotationYaw = 0.0f;

	virtual bool GivePickupTo(APawn* PlayerPawn);
	void PickupWasTaken();
	void Respawn();

	void GenerateRotationYaw();
};
