// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/ActAnimInstance.h"

#include "AbilitySystemInterface.h"
#include "Character/ActCharacterBase.h"

void UActAnimInstance::SetOwnerAbilitySystemComponent(UAbilitySystemComponent* InASC)
{
	OwnerAbilitySystemComp = InASC;
	OwnerAbilitySystemComp->RegisterGenericGameplayTagEvent().AddUObject(this,&UActAnimInstance::UpdateOwnerGameplayTags);
}


void UActAnimInstance::NativeInitializeAnimation()
{
	OwnerPawn = TryGetPawnOwner();
	if (!OwnerPawn) return;
	AActCharacterBase* ActCharacterBase = Cast<AActCharacterBase>(OwnerPawn);
	if (!ActCharacterBase) return;
	ActCharacterBase->AbilitySystemCompHasSet.AddUObject(this,&UActAnimInstance::SetOwnerAbilitySystemComponent);
}

void UActAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//UpdateOwnerGameplayTags();
}
