// Basic Shooter Game. All rights reserved

#include "UI/BSHealthBarWidget.h"
#include "Components/ProgressBar.h"

void UBSHealthBarWidget::SetHealthPercent(float Percent)
{
	if (!HealthProgressBar) return;

	const ESlateVisibility HealthBarVisibility = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent)) //
													 ? ESlateVisibility::Hidden
													 : ESlateVisibility::Visible;

	HealthProgressBar->SetVisibility(HealthBarVisibility);

	const FLinearColor HealthBarColor = Percent > PercentColorThreshold ? HighHealthColor : LowHealthColor;
	HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);

	HealthProgressBar->SetPercent(Percent);
}
