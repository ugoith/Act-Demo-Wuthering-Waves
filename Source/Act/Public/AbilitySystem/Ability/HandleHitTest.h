// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActGameplayAbilityBase.h"
#include "Component/DTManagerComponent.h"
#include "HandleHitTest.generated.h"

struct FActionConfig;
class UPayloadInfo;
class UAbilityTask_WaitGameplayEvent;
class UAbilityAsync_WaitGameplayEvent;
/**
 * 
 */
UCLASS()
class ACT_API UHandleHitTest : public UActGameplayAbilityBase
{
	GENERATED_BODY()
public:
	UHandleHitTest();
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEvent;
	UPROPERTY(BlueprintReadOnly)
	FGameplayEventData HasSentPayloadData;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPayloadInfo> PayloadInfo;
	UPROPERTY(BlueprintReadOnly)
	bool bIsSelf = false;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> DTActionConfig;
	UPROPERTY(BlueprintReadOnly)
	FActionConfig SatisfiedActionConfig;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void ReceiveGameplayEvent(FGameplayEventData PayloadData);
	//接收Event后先执行GE
	void HandleGEToSelf(const FGameplayEventData& PayloadData);
	bool CanPlayBeHitAnim(const FGameplayEventData& PayloadData);
	UFUNCTION(BlueprintImplementableEvent,DisplayName="PlayBeHitAnim")
	void BP_PlayBeHitAnim();
	UFUNCTION(BlueprintImplementableEvent,DisplayName="ReceiveWaitGameplayEvent")
	void BP_ReceiveWaitGameplayEvent(FGameplayEventData PayloadData);
	UFUNCTION(BlueprintCallable)
	bool CheckActionConfigCondition(FName RowName);
	UFUNCTION(BlueprintCallable)
	void LaunchAvatarCharacter();
	UFUNCTION(BlueprintCallable)
	bool GetAllMatchConditionAction(TArray<FActionConfig>& ActionConfigRows);
	UFUNCTION(BlueprintCallable)
	void GetFinalRowStructAccordingToPriority();
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
