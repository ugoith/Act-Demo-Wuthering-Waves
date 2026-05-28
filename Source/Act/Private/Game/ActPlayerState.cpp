// Fill out your copyright notice in the Description page of Project Settings.


#include "Act/Public/Game/ActPlayerState.h"

#include "AbilitySystem/ActAbilitySystemComponent.h"
#include "AbilitySystem/ActAttributeSet.h"
#include "Net/UnrealNetwork.h"

AActPlayerState::AActPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UActAbilitySystemComponent>(FName("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UActAttributeSet>(FName("AttributeSet"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	NetUpdateFrequency = 100.f;;
}

UAbilitySystemComponent* AActPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
