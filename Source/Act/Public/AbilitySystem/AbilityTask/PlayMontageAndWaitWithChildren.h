// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PlayMontageAndWaitWithChildren.generated.h"

/**
 * 
 */
UCLASS()
class ACT_API UPlayMontageAndWaitWithChildren : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category = "Ability|Tasks",meta = (DisplayName = "PlayMontageAndWaitWithChildren",
		HidePin = "OwningAbility",DefaultToSelf = "OwningAbility",BlueprinInternalUseOnly = "TRUE" ))
	static UPlayMontageAndWaitWithChildren* CreatePlayMontageAndWaitWithChildren(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Test;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	AActor* Actor;
	virtual void Activate() override; 
};
