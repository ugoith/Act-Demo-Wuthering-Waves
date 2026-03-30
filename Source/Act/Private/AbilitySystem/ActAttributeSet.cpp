// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ActAttributeSet.h"

#include "Net/UnrealNetwork.h"

void UActAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UActAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	
}

void UActAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	
}
