// Basic Shooter Game. All rights reserved

#include "UI/BSBaseWidget.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"

void UBSBaseWidget::Show() 
{
	PlayAnimation(ShowAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), OpenSound);
}
