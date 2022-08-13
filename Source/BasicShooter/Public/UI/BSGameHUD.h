// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BSCoreTypes.h"
#include "BSGameHUD.generated.h"

class UBSBaseWidget;

UCLASS()
class BASICSHOOTER_API ABSGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TMap<EBSMatchState, UBSBaseWidget*> GameWidgets;

	UPROPERTY()
	UBSBaseWidget* CurrentWidget = nullptr;

	void StartWidgets();

	void SubscribeOnMatchStateChanged();

	void DrawCrossHair();

	void OnMatchStateChanged(EBSMatchState State);
};
