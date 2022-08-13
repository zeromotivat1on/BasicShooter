// Basic Shooter Game. All rights reserved

#include "AI/BSAICharacter.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/BSAIController.h"
#include "UI/BSHealthBarWidget.h"
#include "Components/BSHealthComponent.h"
#include "Components/BSAIWeaponComponent.h"

ABSAICharacter::ABSAICharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<UBSAIWeaponComponent>("WeaponComponent"))

{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ABSAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 360.0f);
	}

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ABSAICharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthWidgetComponent);
}

void ABSAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHealthWidgetVisibility();
}

void ABSAICharacter::OnDeath()
{
	Super::OnDeath();

	const auto BSController = Cast<AAIController>(Controller);
	if (BSController && BSController->BrainComponent)
	{
		BSController->BrainComponent->Cleanup(); //
	}
}

void ABSAICharacter::OnHealthChanged(float NewHealth, float HealthDelta)
{
	Super::OnHealthChanged(NewHealth, HealthDelta);

	const auto HealthBarWidget = Cast<UBSHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if (!HealthBarWidget) return;

	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void ABSAICharacter::UpdateHealthWidgetVisibility()
{
	if (!GetWorld() ||							   //
		!GetWorld()->GetFirstPlayerController() || //
		!GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())
		return;

	const FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
	const double Distance = FVector::Distance(PlayerLocation, GetActorLocation());

	HealthWidgetComponent->SetVisibility(Distance < HealthVisiblityDistance, true);
}
