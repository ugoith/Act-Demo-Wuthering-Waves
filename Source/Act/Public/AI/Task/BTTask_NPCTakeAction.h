// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NPCTakeAction.generated.h"

struct FGameplayAbilitySpec;
struct FAbilityEndedData;
class AActAIControllerBase;
class UAct_ListenGenericGameplayTagChange;
class UGameplayAbility;
/**
 * 
 */
UCLASS(DisplayName= "NPCTakeAction")
class ACT_API UBTTask_NPCTakeAction : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_NPCTakeAction(const FObjectInitializer& ObjectInitializer);
	TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> Actions;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bTest = true;

	bool bIsProcessing = false;
	int32 LastEndActionIndex = -1;
	TWeakObjectPtr<AActAIControllerBase> AIController;
	int32 TakeActionIndex = 0; 
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAct_ListenGenericGameplayTagChange> GameplayTagChangedAsyncTask;

	FDelegateHandle OnAbilityEndedHandle;

	UFUNCTION()
	void Callback_GenericGameplayTagChangeDelegate(FGameplayTag ChangedTag, int32 NewTagCount);
	void OnLastGameplayAbilityEnd(const FAbilityEndedData& AbilityEndedData);
	void TryActivateNextAction();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
	void ResetParams();
};
