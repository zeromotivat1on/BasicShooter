// Basic Shooter Game. All rights reserved

#include "Sound/BSSoundFuncLib.h"
#include "Sound/SoundClass.h"

void UBSSoundFuncLib::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
	if (!SoundClass) return;

	SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

void UBSSoundFuncLib::ToggleSoundClassVolume(USoundClass* SoundClass) 
{
	if (!SoundClass) return;

	const float NewVolume = SoundClass->Properties.Volume > 0.0f ? 0.0f : 1.0f;
	SetSoundClassVolume(SoundClass, NewVolume);
}
