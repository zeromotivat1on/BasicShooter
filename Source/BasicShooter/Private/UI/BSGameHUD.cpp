// Basic Shooter Game. All rights reserved

#include "UI/BSGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/BSBaseWidget.h"
#include "BSGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogBSGameHUD, All, All);

void ABSGameHUD::DrawHUD()
{
	Super::DrawHUD();

	// DrawCrossHair();
}

void ABSGameHUD::BeginPlay()
{
	Super::BeginPlay();

	StartWidgets();

	SubscribeOnMatchStateChanged();
}

void ABSGameHUD::StartWidgets()
{
	GameWidgets.Add(EBSMatchState::InProgress, CreateWidget<UBSBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
	GameWidgets.Add(EBSMatchState::Pause, CreateWidget<UBSBaseWidget>(GetWorld(), PauseWidgetClass));
	GameWidgets.Add(EBSMatchState::GameOver, CreateWidget<UBSBaseWidget>(GetWorld(), GameOverWidgetClass));

	for (const auto& [MatchState, GameWidget] : GameWidgets)
	{
		if (!GameWidget) continue;

		GameWidget->AddToViewport();
		GameWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABSGameHUD::SubscribeOnMatchStateChanged()
{
	if (GetWorld())
	{
		const auto GameMode = Cast<ABSGameModeBase>(GetWorld()->GetAuthGameMode());
		if (!GameMode) return;

		GameMode->OnMatchStateChanged.AddUObject(this, &ABSGameHUD::OnMatchStateChanged);
	}
}

void ABSGameHUD::OnMatchStateChanged(EBSMatchState State)
{
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden); //
	}

	if (GameWidgets.Contains(State))
	{
		CurrentWidget = GameWidgets[State]; //
	}

	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Show();
	}

	UE_LOG(LogBSGameHUD, Display, TEXT("Match state was changed: %s"), *UEnum::GetValueAsString(State));
}

void ABSGameHUD::DrawCrossHair()
{
	const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LineColor = FLinearColor::Green;

	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
