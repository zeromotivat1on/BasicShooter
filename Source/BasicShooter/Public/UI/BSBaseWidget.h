// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSBaseWidget.generated.h"

class USoundCue;

UCLASS()
class BASICSHOOTER_API UBSBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void Show();

protected:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* OpenSound;
};
