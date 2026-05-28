// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityAsync/AbilityAsync_WaitGameplayTagChange.h"

UAbilityAsync_WaitGameplayTagChange* UAbilityAsync_WaitGameplayTagChange::WaitGameplayTagChangeToActor(
	AActor* TargetActor, FGameplayTag Tag, bool OnlyTriggerOnce)
{
	UAbilityAsync_WaitGameplayTagChange* MyObj = NewObject<UAbilityAsync_WaitGameplayTagChange>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->Tag = Tag;
	MyObj->OnlyTriggerOnce = OnlyTriggerOnce;
	return MyObj;
	//MyObj->TargetCount = 1;
	
}

void UAbilityAsync_WaitGameplayTagChange::GameplayTagCallback(const FGameplayTag InTag, int32 NewCount)
{
	if (ShouldBroadcastDelegates())
	{
		OnGameplayEffectTagCountChanged.Broadcast(InTag,NewCount);
		if (OnlyTriggerOnce)
		{
			EndAction();
		}
	}
	else
	{
		EndAction();
	}
}

