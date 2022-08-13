// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/BSBaseWidget.h"
#include "BSPauseWidget.generated.h"

class UButton;

UCLASS()
class BASICSHOOTER_API UBSPauseWidget : public UBSBaseWidget
{
	GENERATED_BODY()

public:

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClearPause();
};
