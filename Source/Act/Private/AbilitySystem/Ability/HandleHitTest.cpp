// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/HandleHitTest.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ActGameplayTags.h"
#include "BlueprintGameplayTagLibrary.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/ActAttributeSet.h"
#include "BlueprintFunctionLibrary/ActBlueprintFunctionLibrary.h"
#include "Component/DTManagerComponent.h"
#include "Component/HitAndDamageComponent.h"
#include "Data/PayloadInfo.h"
#include "GameFramework/Character.h"

UHandleHitTest::UHandleHitTest()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
}

void UHandleHitTest::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{			
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
	WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,EventTags::EventTags_HitSuccess);
	WaitGameplayEvent->EventReceived.AddDynamic(this,&UHandleHitTest::ReceiveGameplayEvent);
	WaitGameplayEvent->Activate();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UHandleHitTest::ReceiveGameplayEvent(FGameplayEventData PayloadData)
{
	HasSentPayloadData = PayloadData;
	UDTManagerComponent* DTManager = CastChecked<UDTManagerComponent>(GetAvatarActorFromActorInfo()->GetComponentByClass(UDTManagerComponent::StaticClass()) );
	DTActionConfig = DTManager->ActionData ;
	HandleGEToSelf(PayloadData);
	if (CanPlayBeHitAnim(PayloadData))
	{
		//GetFinalRowStructAccordingToPriority();
		BP_PlayBeHitAnim();
	}
	BP_ReceiveWaitGameplayEvent(PayloadData);
}

void UHandleHitTest::HandleGEToSelf(const FGameplayEventData& PayloadData)
{
	PayloadInfo = Cast<UPayloadInfo>(PayloadData.OptionalObject);
	if (!PayloadInfo) return;
	bIsSelf = PayloadData.Instigator == GetAvatarActorFromActorInfo();
	//FActiveGameplayEffectHandle ActiveGameplayEffectHandle; 
	for (const FApplyGEInfo& GEInfo : PayloadInfo->ApplyGEInfo)
		{
			if ( GEInfo.Magnitude && GEInfo.Tag.IsValid())
			{
				if (bIsSelf)
				{
					const FGameplayEffectSpecHandle& EffectSpecHandle = MakeOutgoingGameplayEffectSpec(GEInfo.GameplayEffectToSelf);
					const FGameplayEffectSpecHandle& SetByCallerGESpecHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,GEInfo.Tag,GEInfo.Magnitude);
					ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(),CurrentActorInfo,GetCurrentActivationInfo(),SetByCallerGESpecHandle);
				}
				else
				{
					const FGameplayEffectSpecHandle& EffectSpecHandle = MakeOutgoingGameplayEffectSpec(GEInfo.GameplayEffectToTarget);
					const FGameplayEffectSpecHandle& SetByCallerGESpecHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,GEInfo.Tag,GEInfo.Magnitude);
					ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(),CurrentActorInfo,GetCurrentActivationInfo(),SetByCallerGESpecHandle);
				}
			}
			else
			{
				if (bIsSelf)
				{
					const FGameplayEffectSpecHandle& EffectSpecHandle = MakeOutgoingGameplayEffectSpec(GEInfo.GameplayEffectToSelf);
					ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(),CurrentActorInfo,GetCurrentActivationInfo(),EffectSpecHandle);
				}//self
				else
				{
					const FGameplayEffectSpecHandle& EffectSpecHandle = MakeOutgoingGameplayEffectSpec(GEInfo.GameplayEffectToTarget);
					ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(),CurrentActorInfo,GetCurrentActivationInfo(),EffectSpecHandle);
				}//Target
			}//非setByCaller
		}//for
	//在使用ApplyGameplayEffectSpecToOwner后会立即进行尝试应用GE，故后面的逻辑的都是应用后的
	//UE_LOG(LogTemp,Warning,TEXT("CurrentHealth : %f"),GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UActAttributeSet::GetHealthAttribute()));
	
}

bool UHandleHitTest::CanPlayBeHitAnim(const FGameplayEventData& PayloadData) 
{
	if (bIsSelf)
	{
		/*UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(),
			FGameplayTagContainer(BattleActions::BeHit::BattleActions_BeHit),true);*/
		return false;
	}
	else
	{
		FGameplayTagContainer Tags(BattleActions::ArmorState::BattleActions_ArmorState_Armor);
		Tags.AddTagFast(BattleActions::ArmorState::BattleActions_ArmorState_Invincible);
		FGameplayTagContainer OwnedTags;
		GetAvatarActorOwnedGameplayTags(OwnedTags);
		if (UBlueprintGameplayTagLibrary::HasAnyTags(OwnedTags,Tags,true))
		{
			/*UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(),
				FGameplayTagContainer(BattleActions::BeHit::BattleActions_BeHit),true);*/
			return false;
		}
		else
		{
			UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(GetAvatarActorFromActorInfo(),
				FGameplayTagContainer(BattleActions::BeHit::BattleActions_BeHit),true);
			return true;
		}
	}
	
}

bool UHandleHitTest::CheckActionConfigCondition(FName RowName)
{
	const FActionConfig* ActionConfig = DTActionConfig->FindRow<FActionConfig>(RowName,TEXT(""));
	return QueryAvatarActorOwnedTags(ActionConfig->QueryOwnedTag);
}


void UHandleHitTest::LaunchAvatarCharacter()
{
	if (bIsSelf) return;
	if (!PayloadInfo) return ;
	if (PayloadInfo->PhysicConfig.PhysicalActor)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Cast<APawn>(HasSentPayloadData.Instigator);
		AActor* PhysicalActor = GetWorld()->SpawnActor<AActor>(SpawnParams);	
		const FHitResult& HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(HasSentPayloadData.TargetData,0);
		PhysicalActor->SetActorLocation(HitResult.Location);
		return ;
	}
	if (PayloadInfo->PhysicConfig.LaunchVelocityMultiplier == FVector()) return;
	if (!AbilityOwnerCharacter)
	{
		ECastResult CastResult = ECastResult::CastFailed;
		GetAndCastAbilityOwnerCharacter(CastResult);
		if (CastResult == ECastResult::CastFailed) return;
	}
	const FVector& InstigatorLocation = HasSentPayloadData.Instigator->GetActorLocation();
	const FVector& AvatarLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector& InstigatorToTarget = AvatarLocation - InstigatorLocation;
	FVector ForwardVector,RightVector,UpVector;
	UActBlueprintFunctionLibrary::GetThreeDirection(InstigatorToTarget.Rotation(),ForwardVector,RightVector,UpVector);
	const FVector& FinalLaunchVelocity = ForwardVector * PayloadInfo->PhysicConfig.LaunchVelocityMultiplier.X +
		RightVector * PayloadInfo->PhysicConfig.LaunchVelocityMultiplier.Y +
			UpVector * PayloadInfo->PhysicConfig.LaunchVelocityMultiplier.Z;
	AbilityOwnerCharacter->LaunchCharacter(FinalLaunchVelocity,PayloadInfo->PhysicConfig.bXYOverride,PayloadInfo->PhysicConfig.bZOverride);
	
}

bool UHandleHitTest::GetAllMatchConditionAction(TArray<FActionConfig>& ActionConfigRows)
{
	TArray<FActionConfig*> ActionConfigs;
	DTActionConfig->GetAllRows<FActionConfig>(TEXT(""),ActionConfigs);
	for (FActionConfig* ActionConfig : ActionConfigs)
	{
		if (QueryAvatarActorOwnedTags(ActionConfig->QueryOwnedTag))
		{
			ActionConfigRows.Add(*ActionConfig);
		}
	}
	return !ActionConfigRows.IsEmpty();
}

void UHandleHitTest::GetFinalRowStructAccordingToPriority()
{
	TArray<FActionConfig> ActionConfigs;
	if (GetAllMatchConditionAction(ActionConfigs))
	{
		uint8 MaxPriority = 0;
		for (const FActionConfig& ActionConfig : ActionConfigs)
		{
			if (ActionConfig.Priority > MaxPriority)
			{
				MaxPriority = ActionConfig.Priority;
				SatisfiedActionConfig = ActionConfig;
			}
		}
		return;
	}
	SatisfiedActionConfig = FActionConfig();
}

void UHandleHitTest::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	/*if (IsEndAbilityValid(Handle,ActorInfo))
	{
		if (IsValid(WaitGameplayEvent))
		WaitGameplayEvent->EndTask();
	}*/
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
