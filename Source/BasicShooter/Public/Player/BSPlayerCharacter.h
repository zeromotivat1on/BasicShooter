// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/BSBaseCharacter.h"
#include "BSPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USphereComponent;

UCLASS()
class BASICSHOOTER_API ABSPlayerCharacter : public ABSBaseCharacter
{
	GENERATED_BODY()

public:
	ABSPlayerCharacter(const FObjectInitializer& ObjInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CameraCollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Utils")
	bool DisableFireWhileRunning = true;

	UPROPERTY(EditDefaultsOnly, Category = "Utils")
	bool DisableRunWhileMovingRight = true;

	UPROPERTY(EditDefaultsOnly, Category = "Utils", //
		meta = (EditCondition = "DisableRunWhileMovingRight", ClampMin = "0.0", ClampMax = "90.0"))
	float AngleToStopRun = 30.0f;

	virtual void BeginPlay() override;
	virtual void OnDeath() override;

public:
	virtual bool IsRunning() const override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	bool WantsToRun = false;
	bool IsMovingForward = false;

	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void OnStartRunning();
	void OnStopRunning();

	void LookUp(float Amount);
	void TurnAround(float Amount);

	virtual void Jump() override;

	void StartFire();
	void StopFire();

	void NextWeapon();
	void Reload();

	void Zoom(bool Enabled);

	void TryDisableRunWhileMovingRight();

	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCameraCollisionEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckCameraOverlap();
};
