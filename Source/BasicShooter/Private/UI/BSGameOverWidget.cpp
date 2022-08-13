// Basic Shooter Game. All rights reserved

#include "UI/BSGameOverWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "BSUtils.h"
#include "BSGameInstance.h"
#include "BSGameModeBase.h"
#include "Player/BSPlayerState.h"
#include "UI/BSPlayerStatRowWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogBSGameOverWidget, All, All);

void UBSGameOverWidget::NativeOnInitialized() 
{
	Super::NativeOnInitialized();

	SubscribeOnMatchStateChanged();

	if (RestartLevelButton) 
	{ 
		RestartLevelButton->OnClicked.AddDynamic(this, &UBSGameOverWidget::OnRestartLevel); //
	}
}

void UBSGameOverWidget::SubscribeOnMatchStateChanged()
{
	if (GetWorld())
	{
		const auto GameMode = Cast<ABSGameModeBase>(GetWorld()->GetAuthGameMode());
		if (!GameMode) return;

		GameMode->OnMatchStateChanged.AddUObject(this, &UBSGameOverWidget::OnMatchStateChanged);
	}
}

void UBSGameOverWidget::OnMatchStateChanged(EBSMatchState State)
{
	if (State == EBSMatchState::GameOver)
	{
		UpdatePlayerStat(); //
	}
}

void UBSGameOverWidget::UpdatePlayerStat()
{
	if (!GetWorld() || !PlayerStatBox) return;

	PlayerStatBox->ClearChildren();

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It) 
	{ 
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<ABSPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;

		const auto PlayerStatRowWidget = CreateWidget<UBSPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
		if (!PlayerStatRowWidget) continue;

		PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
		PlayerStatRowWidget->SetKills(BSUtils::TextFromInt(PlayerState->GetKillsNum()));
		PlayerStatRowWidget->SetDeaths(BSUtils::TextFromInt(PlayerState->GetDeathsNum()));
		PlayerStatRowWidget->SetTeam(BSUtils::TextFromInt(PlayerState->GetTeamID()));
		PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
		PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());

		PlayerStatBox->AddChild(PlayerStatRowWidget);
	}
}

void UBSGameOverWidget::OnRestartLevel()
{
	if (!GetWorld()) return;

	const auto BSGameInstance = GetWorld()->GetGameInstance<UBSGameInstance>();
	if (!BSGameInstance) return;

	UGameplayStatics::OpenLevel(this, BSGameInstance->GetStartupLevel().LevelName);
}
