// Basic Shooter Game. All rights reserved

#include "BSGameInstance.h"
#include "Sound/BSSoundFuncLib.h"

void UBSGameInstance::ToggleVolume() 
{
	UBSSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}
