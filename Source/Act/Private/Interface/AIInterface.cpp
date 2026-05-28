// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/AIInterface.h"


// Add default functionality here for any IAIInterface functions that are not pure virtual.
bool IAIInterface::ActivateControlledAbilities(TSubclassOf<UGameplayAbility> Ability, bool bAllowRemoteActivation)
{
	return false;
}
