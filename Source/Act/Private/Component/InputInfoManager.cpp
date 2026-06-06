// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InputInfoManager.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UInputInfoManager::UInputInfoManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInputInfoManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInputInfoManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInputInfoManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,PreInputTag);
	DOREPLIFETIME(ThisClass,bCanPreInput);
}

void UInputInfoManager::UpdatePreInput(FGameplayTag TagToSet)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("PreInputTagMessage : %s"),Targe));
	if (!bCanPreInput) return;
	
	if (PreInputTagConsumed.IsEmpty() || !PreInputTagConsumed.Contains(TagToSet))
	{
		PreInputTagConsumed.Add(TagToSet,false);
		PreInputTag = TagToSet;
	}
	else
	{
		//该tag未使用
		if (!PreInputTagConsumed.Find(TagToSet))
		{
			PreInputTag = TagToSet;
		}
	}
}

void UInputInfoManager::SendPreInputEvent()
{
	FGameplayEventData PayloadData;
	PayloadData.Instigator = GetOwner();
	PayloadData.EventTag = PreInputTag;
	PreInputTagConsumed.Add(PreInputTag,true);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(),PreInputTag,PayloadData);
}

void UInputInfoManager::StartPreInput()
{
	bCanPreInput = true;
	PreInputTag = FGameplayTag();
	//PreInputTagConsumed.Empty();
}

void UInputInfoManager::ResetAllParams()
{
	bCanPreInput = false;
	PreInputTag = FGameplayTag();
	PreInputTagConsumed.Empty();
}

void UInputInfoManager::UpdateTriggerThresholds(float DeltaTime)
{
	if (TriggerThresholds.IsEmpty()) return;
	for (FTriggerThreshold& TriggerThreshold : TriggerThresholds)
	{
		if (TriggerThreshold.Threshold <= TriggerThreshold.TimePassed)
			continue;
		TriggerThreshold.TimePassed += DeltaTime;
		if (TriggerThreshold.TimePassed >= TriggerThreshold.Threshold)
		{
			UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(GetOwner(),TriggerThreshold.AddTagsWhenExceedThreshold,true);
			HasAddedTagsWhenExceedThreshold.AppendTags(TriggerThreshold.AddTagsWhenExceedThreshold);
		}
	}
	
}

void UInputInfoManager::ResetTriggerThresholds()
{
	TriggerThresholds.Empty();
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetOwner(),GetHasAddedTagsWhenExceedThreshold(),true);
}


