// Basic Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BSAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotiiedSignature, USkeletalMeshComponent*);

UCLASS()
class BASICSHOOTER_API UBSAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FOnNotiiedSignature OnNotified;

	virtual void Notify(
		USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
