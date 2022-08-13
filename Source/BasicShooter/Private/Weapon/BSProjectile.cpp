// Basic Shooter Game. All rights reserved

#include "Weapon/BSProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/BSWeaponFXComponent.h"

ABSProjectile::ABSProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true; // return phys material
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;

	WeaponFXComponent = CreateDefaultSubobject<UBSWeaponFXComponent>("WeaponFXComponent");
}

void ABSProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(CollisionComponent);

	check(MovementComponent);
	check(MovementComponent->InitialSpeed >= 0.0f);
	check(MovementComponent->ProjectileGravityScale == 0.0f);

	check(WeaponFXComponent);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABSProjectile::OnProjectileHit);

	SetLifeSpan(ProjectileLifeSpan);
}

void ABSProjectile::OnProjectileHit(
	UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;

	MovementComponent->StopMovementImmediately();

	UGameplayStatics::ApplyRadialDamage(GetWorld(), //
		DamageAmount,								//
		GetActorLocation(),							//
		DamageRadius,								//
		UDamageType::StaticClass(),					//
		{},											// Actors to ignore
		this,										//
		GetController(),							//
		DoFullDamage);

	DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, ProjectileLifeSpan);
	WeaponFXComponent->PlayImpactFX(Hit);

	Destroy();
}

AController* ABSProjectile::GetController() const
{
	APawn* const Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}