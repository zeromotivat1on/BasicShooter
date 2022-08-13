// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/BSBaseWidget.h"
#include "BSCoreTypes.h"
#include "BSMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class UBSLevelItemWidget;
class UBSGameInstance;
class USoundCue;

UCLASS()
class BASICSHOOTER_API UBSMenuWidget : public UBSBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LevelItemsBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	TSubclassOf<UUserWidget> LevelItemWidgetClass;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HideAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* StartGameSound;

	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	UPROPERTY()
	TArray<UBSLevelItemWidget*> LevelItemWidgets;

	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnQuitGame();

	void InitLevelItems();
	void OnLevelSelected(const FLevelData& Data);

	UBSGameInstance* GetBSGameInstance() const;
};
