// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Act_ListenGenericGameplayTagChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGenericGameplayTagChange,FGameplayTag,ChangedTag,int32,NewTagCount);

/**
 * 
 */
UCLASS(BlueprintType,meta = (ExposedAsyncProxy = AsyncTask))
class ACT_API UAct_ListenGenericGameplayTagChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true",DisplayName="ListenGenericGameplayTagChange"))
	static UAct_ListenGenericGameplayTagChange* CreateListenGenericGameplayTagChange(UAbilitySystemComponent* AbilitySystemComponent);
	
	TWeakObjectPtr<UAbilitySystemComponent> ASC;
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTagChange ReceiveGenericGameplayTagChange;
	
	void BroadcastGenericGameplayTagChange(FGameplayTag ChangedTag,int32 NewTagCount);

	UFUNCTION(BlueprintCallable)
	void EndTask();
};
