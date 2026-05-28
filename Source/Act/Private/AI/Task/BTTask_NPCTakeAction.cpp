// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_NPCTakeAction.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "AbilitySystem/Act_ListenGenericGameplayTagChange.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Controller/ActAIControllerBase.h"



UBTTask_NPCTakeAction::UBTTask_NPCTakeAction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "NPCTakeAction";
	bNotifyTick = true;
	// all blueprint based nodes must create instances
	bCreateNodeInstance = true;
	
}

void UBTTask_NPCTakeAction::Callback_GenericGameplayTagChangeDelegate(FGameplayTag ChangedTag, int32 NewTagCount)
{
	TryActivateNextAction();
}

void UBTTask_NPCTakeAction::OnLastGameplayAbilityEnd(const FAbilityEndedData& AbilityEndedData)
{
	++LastEndActionIndex;
	if (bTest)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,
			FString::Printf(TEXT("LastEndActionIndex: %d ; TakeActionIndex: %d"),LastEndActionIndex,TakeActionIndex)) ;
	}
	if (LastEndActionIndex < TakeActionIndex - 1) return; //RetriggerAbility时, 不触发EndAbility

	ResetParams();
	if (BehaviorTreeComp.IsValid())
	{
		FinishLatentTask(*BehaviorTreeComp.Get(), EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTTask_NPCTakeAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (bTest)
	{
		GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Emerald,FString("ExecuteTask"));
		GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Emerald,FString(TEXT("BTOwner: ")) + OwnerComp.GetOwner()->GetName()) ;
		GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Emerald,FString(TEXT("BTAIOwner: ")) + OwnerComp.GetAIOwner()->GetName()) ;
	}
	BehaviorTreeComp = &OwnerComp;
	AIController = Cast<AActAIControllerBase>(OwnerComp.GetAIOwner());
	if (!AIController.IsValid()) return EBTNodeResult::Failed;
	if (!AIController->AbilitySystemComponent.IsValid()) return EBTNodeResult::Failed;
	if (Actions.Num() == 0) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = AIController->AbilitySystemComponent.Get();

	// Bind to ability end delegate to detect when the last GA finishes
	OnAbilityEndedHandle = ASC->OnAbilityEnded.AddUObject(this, &UBTTask_NPCTakeAction::OnLastGameplayAbilityEnd);

	GameplayTagChangedAsyncTask = UAct_ListenGenericGameplayTagChange::CreateListenGenericGameplayTagChange(ASC);
	GameplayTagChangedAsyncTask->ReceiveGenericGameplayTagChange.AddDynamic(this, &UBTTask_NPCTakeAction::Callback_GenericGameplayTagChangeDelegate);

	// Reset state and activate first action
	TakeActionIndex = 0;
	LastEndActionIndex = -1;
	TryActivateNextAction();

	return EBTNodeResult::InProgress;
}

void UBTTask_NPCTakeAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bTest)
		GEngine->AddOnScreenDebugMessage(-1,DeltaSeconds,FColor::Emerald,FString("TickTask"));
	BehaviorTreeComp = BehaviorTreeComp.IsValid() ? BehaviorTreeComp : &OwnerComp ;
	//if (OwnerComp.GetOwner());
	
}

EBTNodeResult::Type UBTTask_NPCTakeAction::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (bTest)
	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Emerald,FString("AbortTask"));
	ResetParams();
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_NPCTakeAction::ResetParams()
{
	LastEndActionIndex = -1;
	TakeActionIndex = 0;
	bIsProcessing = false;

	if (GameplayTagChangedAsyncTask)
	{
		GameplayTagChangedAsyncTask->EndTask();
		GameplayTagChangedAsyncTask = nullptr;
	}

	if (OnAbilityEndedHandle.IsValid() && AIController.IsValid())
	{
		UAbilitySystemComponent* ASC = AIController->AbilitySystemComponent.Get();
		if (ASC)
		{
			ASC->OnAbilityEnded.Remove(OnAbilityEndedHandle);
			OnAbilityEndedHandle.Reset();
		}
	}
}

void UBTTask_NPCTakeAction::TryActivateNextAction()
{
	if (!AIController.IsValid()) return;
	if (!Actions.IsValidIndex(TakeActionIndex)) return;
	if (bIsProcessing) return;//重入保护

	bIsProcessing = true;

	TSubclassOf<UGameplayAbility> AbilityClass = Actions[TakeActionIndex];
	if (AIController->ActivateControlledAbilities(AbilityClass, true))
	{
		++TakeActionIndex;
	}

	bIsProcessing = false;
}
