// Basic Shooter Game. All rights reserved

#include "Menu/UI/BSMenuHUD.h"
#include "UI/BSBaseWidget.h"

void ABSMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		const auto MenuWidget = CreateWidget<UBSBaseWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
			MenuWidget->Show();
		}
	}
}
