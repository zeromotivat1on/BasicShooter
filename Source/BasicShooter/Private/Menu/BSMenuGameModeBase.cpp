// Basic Shooter Game. All rights reserved

#include "Menu/BSMenuGameModeBase.h"
#include "Menu/BSMenuPlayerController.h"
#include "Menu/UI/BSMenuHUD.h"

ABSMenuGameModeBase::ABSMenuGameModeBase() 
{
	PlayerControllerClass = ABSMenuPlayerController::StaticClass();
	HUDClass = ABSMenuHUD::StaticClass();
}