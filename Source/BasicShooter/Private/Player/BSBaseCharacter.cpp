// Basic Shooter Game. All rights reserved

#include "Player/BSBaseCharacter.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BSHealthComponent.h"
#include "Components/BSWeaponComponent.h"
#include "Components/BSCharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

ABSBaseCharacter::ABSBaseCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<UBSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UBSHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<UBSWeaponComponent>("WeaponComponent");
}

void ABSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(GetCharacterMovement());
	check(GetMesh());

	OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
	HealthComponent->OnDeath.AddUObject(this, &ABSBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ABSBaseCharacter::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &ABSBaseCharacter::OnGoundLanded);
}

void ABSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABSBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
	const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	if (!MaterialInst) return;

	MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}

void ABSBaseCharacter::TurnOff() 
{
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);

	Super::TurnOff();
}

void ABSBaseCharacter::Reset() 
{
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);

	Super::Reset();
}

bool ABSBaseCharacter::IsRunning() const
{
	return false;
}

float ABSBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;

	const FVector3d VelocityNormal = GetVelocity().GetSafeNormal();
	const FVector3d ForwardVector = GetActorForwardVector();
	const FVector3d CrossProduct = FVector::CrossProduct(ForwardVector, VelocityNormal);
	const float AngleBetween = FMath::Acos(FVector::DotProduct(ForwardVector, VelocityNormal));
	const float Degrees = FMath::RadiansToDegrees(AngleBetween);

	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ABSBaseCharacter::OnDeath()
{
	//PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifespanOnDeath);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void ABSBaseCharacter::OnHealthChanged(float NewHealth, float HealthDelta)
{
	
}

void ABSBaseCharacter::OnGoundLanded(const FHitResult& Hit)
{
	const float FallVelocityZ = GetVelocity().Z * -1;
	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const float FallDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	TakeDamage(FallDamage, FPointDamageEvent(), nullptr, nullptr);
}
