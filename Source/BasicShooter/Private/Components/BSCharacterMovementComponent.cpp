// Basic Shooter Game. All rights reserved


#include "Components/BSCharacterMovementComponent.h"
#include "Player/BSBaseCharacter.h"

float UBSCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ABSBaseCharacter* Player = Cast<ABSBaseCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
