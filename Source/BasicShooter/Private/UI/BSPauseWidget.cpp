// Basic Shooter Game. All rights reserved

#include "UI/BSPauseWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

void UBSPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &UBSPauseWidget::OnClearPause); //
	}
}

void UBSPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->ClearPause();
}
