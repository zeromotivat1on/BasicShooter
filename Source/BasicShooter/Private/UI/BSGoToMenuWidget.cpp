// Basic Shooter Game. All rights reserved

#include "UI/BSGoToMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "BSGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogBSGoToMenuWidget, All, All);

void UBSGoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GoToMenuButton)
	{
		GoToMenuButton->OnClicked.AddDynamic(this, &UBSGoToMenuWidget::OnGoToMenu); //
	}
}

void UBSGoToMenuWidget::OnGoToMenu() 
{
	if (!GetWorld()) return;

	const auto BSGameInstance = GetWorld()->GetGameInstance<UBSGameInstance>();
	if (!BSGameInstance) return;

	if (BSGameInstance->GetMenuLevelName().IsNone())
	{
		UE_LOG(LogBSGoToMenuWidget, Error, TEXT("Menu level name is NONE"));
		return;
	}

	UGameplayStatics::OpenLevel(this, BSGameInstance->GetMenuLevelName());
}
