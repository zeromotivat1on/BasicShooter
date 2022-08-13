// Basic Shooter Game. All rights reserved

#include "Player/BSPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BSWeaponComponent.h"

ABSPlayerCharacter::ABSPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.0f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ABSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(CameraCollisionComponent);

	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABSPlayerCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABSPlayerCharacter::OnCameraCollisionEndOverlap);
}

void ABSPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCameraOverlap();
}

void ABSPlayerCharacter::OnCameraCollisionEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckCameraOverlap();
}

void ABSPlayerCharacter::CheckCameraOverlap()
{
	const bool HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(HideMesh);

	TArray<USceneComponent*> MeshChildren;
	GetMesh()->GetChildrenComponents(true, MeshChildren);

	for (const auto& MeshChild : MeshChildren)
	{
		const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
		if (MeshChildGeometry)
		{
			MeshChildGeometry->SetOwnerNoSee(HideMesh); //
		}
	}
}

void ABSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABSPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABSPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ABSPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnAround", this, &ABSPlayerCharacter::TurnAround);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABSPlayerCharacter::Jump);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ABSPlayerCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ABSPlayerCharacter::OnStopRunning);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABSPlayerCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABSPlayerCharacter::StopFire);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &ABSPlayerCharacter::NextWeapon);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABSPlayerCharacter::Reload);

	DECLARE_DELEGATE_OneParam(FZoomInputSignature, bool);
	PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Pressed, this, &ABSPlayerCharacter::Zoom, true);
	PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Released, this, &ABSPlayerCharacter::Zoom, false);
}

void ABSPlayerCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f;
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ABSPlayerCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f) return;

	TryDisableRunWhileMovingRight();
	AddMovementInput(GetActorRightVector(), Amount);
}

void ABSPlayerCharacter::LookUp(float Amount)
{
	if (Amount == 0.0f) return;
	AddControllerPitchInput(Amount);
}

void ABSPlayerCharacter::TurnAround(float Amount)
{
	if (Amount == 0.0f) return;
	AddControllerYawInput(Amount);
}

void ABSPlayerCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void ABSPlayerCharacter::OnStopRunning()
{
	WantsToRun = false;
}

void ABSPlayerCharacter::Jump()
{
	Super::Jump();
}

void ABSPlayerCharacter::StartFire()
{
	if (IsRunning() && DisableFireWhileRunning) return;
	WeaponComponent->StartFire();
}

void ABSPlayerCharacter::StopFire()
{
	if (IsRunning() && DisableFireWhileRunning) return;
	WeaponComponent->StopFire();
}

void ABSPlayerCharacter::NextWeapon()
{
	WeaponComponent->NextWeapon();
}

void ABSPlayerCharacter::Reload()
{
	WeaponComponent->Reload();
}

void ABSPlayerCharacter::Zoom(bool Enabled) 
{
	WeaponComponent->Zoom(Enabled);
}

void ABSPlayerCharacter::TryDisableRunWhileMovingRight()
{
	if (IsRunning()					  //
		&& DisableRunWhileMovingRight //
		&& FMath::Abs(GetMovementDirection()) > AngleToStopRun)
	{
		WantsToRun = false; //
	}
}

void ABSPlayerCharacter::OnDeath()
{
	Super::OnDeath();
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating); //
	}
}

bool ABSPlayerCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}