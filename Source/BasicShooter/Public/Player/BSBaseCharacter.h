// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BSBaseCharacter.generated.h"

class UBSHealthComponent;
class UBSWeaponComponent;
class USoundCue;

UCLASS()
class BASICSHOOTER_API ABSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABSBaseCharacter(const FObjectInitializer& ObjInit);

	UBSHealthComponent* GetHealthComponent() const { return HealthComponent; }
	UBSWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBSHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBSWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifespanOnDeath = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	FName MaterialColorName = "Paint Color";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* DeathSound;

	virtual void BeginPlay() override;

	virtual void OnDeath();
	virtual void OnHealthChanged(float NewHealth, float HealthDelta);

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

	virtual void Tick(float DeltaTime) override;

	void SetPlayerColor(const FLinearColor& Color);

	virtual void TurnOff() override;
	virtual void Reset() override;

private:
	UFUNCTION()
	void OnGoundLanded(const FHitResult& Hit);
};
