// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Act_ListenGenericGameplayTagChange.h"

UAct_ListenGenericGameplayTagChange* UAct_ListenGenericGameplayTagChange::CreateListenGenericGameplayTagChange(
	UAbilitySystemComponent* AbilitySystemComponent)
{	
	UAct_ListenGenericGameplayTagChange* MyObj = NewObject<UAct_ListenGenericGameplayTagChange>();
	MyObj->ASC = AbilitySystemComponent;
	if (!AbilitySystemComponent)
	{
		MyObj->RemoveFromRoot();
		return nullptr;	
	}
	
	MyObj->ASC->RegisterGenericGameplayTagEvent().AddUObject(MyObj,&UAct_ListenGenericGameplayTagChange::BroadcastGenericGameplayTagChange);
	return MyObj;
}

void UAct_ListenGenericGameplayTagChange::BroadcastGenericGameplayTagChange(FGameplayTag ChangedTag, int32 NewTagCount)
{
	ReceiveGenericGameplayTagChange.Broadcast(ChangedTag,NewTagCount);
}

void UAct_ListenGenericGameplayTagChange::EndTask()
{
	if (ASC.IsValid())
	{
		ASC->RegisterGenericGameplayTagEvent().RemoveAll(this);
	}
	SetReadyToDestroy();
	MarkAsGarbage();
}
