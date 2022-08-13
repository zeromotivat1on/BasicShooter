// Basic Shooter Game. All rights reserved

#include "Menu/BSMenuPlayerController.h"

void ABSMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}
