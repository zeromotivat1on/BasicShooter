// Basic Shooter Game. All rights reserved

#include "Menu/UI/BSMenuWidget.h"
#include "Sound/SoundCue.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BSGameInstance.h"
#include "Menu/UI/BSLevelItemWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogBSMenuWidget, All, All);

void UBSMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UBSMenuWidget::OnStartGame); //
	}

	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UBSMenuWidget::OnQuitGame); //
	}

	InitLevelItems();
}

void UBSMenuWidget::InitLevelItems()
{
	const auto BSGameInstance = GetBSGameInstance();
	if (!BSGameInstance) return;

	checkf(BSGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty"));

	if (!LevelItemsBox) return;
	LevelItemsBox->ClearChildren();

	for (const auto& LevelData : BSGameInstance->GetLevelsData())
	{
		const auto LevelItemWidget = CreateWidget<UBSLevelItemWidget>(GetWorld(), LevelItemWidgetClass);
		if (!LevelItemWidget) continue;

		LevelItemWidget->SetLevelData(LevelData);
		LevelItemWidget->OnLevelSelected.AddUObject(this, &UBSMenuWidget::OnLevelSelected);

		LevelItemsBox->AddChild(LevelItemWidget);
		LevelItemWidgets.Add(LevelItemWidget);
	}

	if (BSGameInstance->GetStartupLevel().LevelName.IsNone())
	{
		OnLevelSelected(BSGameInstance->GetLevelsData()[0]); //
	}
	else
	{
		OnLevelSelected(BSGameInstance->GetStartupLevel()); //
	}
}

void UBSMenuWidget::OnLevelSelected(const FLevelData& Data)
{
	const auto BSGameInstance = GetBSGameInstance();
	if (!BSGameInstance) return;

	BSGameInstance->SetStartupLevel(Data);

	for (const auto& LevelItemWidget : LevelItemWidgets)
	{
		if (!LevelItemWidget) continue;

		const bool IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
		LevelItemWidget->SetSelected(IsSelected);
	}
}

void UBSMenuWidget::OnStartGame()
{
	PlayAnimation(HideAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void UBSMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) 
{
	if (Animation != HideAnimation) return;

	const auto BSGameInstance = GetBSGameInstance();
	if (!BSGameInstance) return;

	UGameplayStatics::OpenLevel(this, BSGameInstance->GetStartupLevel().LevelName);
}

void UBSMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

UBSGameInstance* UBSMenuWidget::GetBSGameInstance() const
{
	if (!GetWorld()) return nullptr;
	return GetWorld()->GetGameInstance<UBSGameInstance>();
}
