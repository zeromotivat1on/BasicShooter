#pragma once
#include "Player/BSPlayerState.h"

class BSUtils
{
public:
	template <typename T> //
	static T* GetBSPlayerComponent(APawn* PlayerPawn)
	{
		// static_assert(TIsDerivedFrom<T, UActorComponent>::IsDerived, "T must inherit from UActorComponent");
		if (!PlayerPawn) return nullptr;

		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}

	bool static AreEnemies(AController* Controller1, AController* Controller2)
	{
		if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;

		const auto PlayerState1 = Cast<ABSPlayerState>(Controller1->PlayerState);
		const auto PlayerState2 = Cast<ABSPlayerState>(Controller2->PlayerState);

		return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
	}

	static FText TextFromInt(int32 Number) { return FText::FromString(FString::FromInt(Number)); }
};