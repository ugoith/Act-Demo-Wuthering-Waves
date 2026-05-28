// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayTag.h"
#include "AbilityAsync_WaitGameplayTagChange.generated.h"

/**
 * 
 */

UCLASS()
class ACT_API UAbilityAsync_WaitGameplayTagChange : public UAbilityAsync_WaitGameplayTag
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityAsync_WaitGameplayTagChange* WaitGameplayTagChangeToActor(AActor* TargetActor, FGameplayTag Tag, bool OnlyTriggerOnce=false);
	
	virtual void GameplayTagCallback(const FGameplayTag Tag, int32 NewCount) override;
	
	FOnGameplayEffectTagCountChanged OnGameplayEffectTagCountChanged;
	//virtual void BroadcastDelegate() override;
};
