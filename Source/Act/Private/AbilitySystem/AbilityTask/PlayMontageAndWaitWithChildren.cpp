// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/PlayMontageAndWaitWithChildren.h"

UPlayMontageAndWaitWithChildren* UPlayMontageAndWaitWithChildren::CreatePlayMontageAndWaitWithChildren(
	UGameplayAbility* OwningAbility)
{
	UPlayMontageAndWaitWithChildren* MyObj = NewObject<UPlayMontageAndWaitWithChildren>(OwningAbility);
	return MyObj;
}

void UPlayMontageAndWaitWithChildren::Activate()
{
	Super::Activate();
	
}
