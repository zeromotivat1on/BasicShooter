// Basic Shooter Game. All rights reserved

#include "UI/BSPlayerStatRowWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UBSPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	if (!PlayerNameTextBlock) return;
	PlayerNameTextBlock->SetText(Text);
}

void UBSPlayerStatRowWidget::SetKills(const FText& Text) 
{
	if (!KillsTextBlock) return;
	KillsTextBlock->SetText(Text);
}

void UBSPlayerStatRowWidget::SetDeaths(const FText& Text)
{
	if (!DeathsTextBlock) return;
	DeathsTextBlock->SetText(Text);
}

void UBSPlayerStatRowWidget::SetTeam(const FText& Text)
{
	if (!TeamTextBlock) return;
	TeamTextBlock->SetText(Text);
}

void UBSPlayerStatRowWidget::SetPlayerIndicatorVisibility(bool Visible)
{
	if (!PlayerIndicatorImage) return;
	PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UBSPlayerStatRowWidget::SetTeamColor(const FLinearColor& Color)
{
	if (!TeamImage) return;

	TeamImage->SetColorAndOpacity(Color);
}
