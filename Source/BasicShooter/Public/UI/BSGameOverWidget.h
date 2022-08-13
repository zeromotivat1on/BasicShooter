// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/BSBaseWidget.h"
#include "BSCoreTypes.h"
#include "BSGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class BASICSHOOTER_API UBSGameOverWidget : public UBSBaseWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerStatBox;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartLevelButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

	virtual void NativeOnInitialized() override;

private:
	void SubscribeOnMatchStateChanged();
	void OnMatchStateChanged(EBSMatchState State);

	void UpdatePlayerStat();

	UFUNCTION()
	void OnRestartLevel();
};
