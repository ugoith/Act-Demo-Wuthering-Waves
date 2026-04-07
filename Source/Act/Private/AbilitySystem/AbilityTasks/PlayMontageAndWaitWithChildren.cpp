// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/AbilityTasks/PlayMontageAndWaitWithChildren.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PlayMontageAndWaitWithChildren)

static bool GUseAggressivePlayMontageAndWaitEndTask = true;
static FAutoConsoleVariableRef CVarAggressivePlayMontageAndWaitEndTask(TEXT("AbilitySystem.PlayMontage.AggressiveEndTask"), GUseAggressivePlayMontageAndWaitEndTask, TEXT("This should be set to true in order to avoid multiple callbacks off an AbilityTask_PlayMontageAndWait node"));

void UPlayMontageAndWaitWithChildren::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	const bool bPlayingThisMontage = (Montage == MontageToPlay) && Ability && Ability->GetCurrentMontage() == MontageToPlay;
	if (bPlayingThisMontage)
	{
		// Reset AnimRootMotionTranslationScale
		ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
		if (Character && (Character->GetLocalRole() == ROLE_Authority ||
							(Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
		{
			Character->SetAnimRootMotionTranslationScale(1.f);
		}
	}

	if (bPlayingThisMontage && (bInterrupted || !bAllowInterruptAfterBlendOut))
	{
		if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
		{
			ASC->ClearAnimatingAbility(Ability);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (bInterrupted)
		{
			OnInterrupted.Broadcast();

			if (GUseAggressivePlayMontageAndWaitEndTask)
			{
				EndTask();
			}
		}
		else
		{
			OnBlendOut.Broadcast();
		}
	}
}

void UPlayMontageAndWaitWithChildren::OnMontageInterrupted()
{
	// Call the new function
	OnGameplayAbilityCancelled();
}

void UPlayMontageAndWaitWithChildren::OnGameplayAbilityCancelled()
{
	if (StopPlayingMontage() || bAllowInterruptAfterBlendOut)
	{
		// Let the BP handle the interrupt as well
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.Broadcast();
		}
	}

	if (GUseAggressivePlayMontageAndWaitEndTask)
	{
		EndTask();
	}
}

void UPlayMontageAndWaitWithChildren::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.Broadcast();
		}
	}

	EndTask();
}

UPlayMontageAndWaitWithChildren* UPlayMontageAndWaitWithChildren::CreatePlayMontageAndWaitWithChildren(UGameplayAbility* OwningAbility,
	FName TaskInstanceName, UAnimMontage *MontageToPlay, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale, float StartTimeSeconds, bool bAllowInterruptAfterBlendOut)
{

	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UPlayMontageAndWaitWithChildren* MyObj = NewAbilityTask<UPlayMontageAndWaitWithChildren>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->bAllowInterruptAfterBlendOut = bAllowInterruptAfterBlendOut;
	MyObj->StartTimeSeconds = StartTimeSeconds;
	
	return MyObj;
}

void UPlayMontageAndWaitWithChildren::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	bool bPlayedMontage = false;

	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			if (ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection, StartTimeSeconds) > 0.f)
			{
				PlayMontageWithSubSkeletalMesh();
				// Playing a montage could potentially fire off a callback into game code which could kill this ability! Early out if we are  pending kill.
				if (ShouldBroadcastAbilityTaskDelegates() == false)
				{
					return;
				}

				InterruptedHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UPlayMontageAndWaitWithChildren::OnGameplayAbilityCancelled);

				BlendingOutDelegate.BindUObject(this, &UPlayMontageAndWaitWithChildren::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UPlayMontageAndWaitWithChildren::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
				if (Character && (Character->GetLocalRole() == ROLE_Authority ||
								  (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
				{
					Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
					
				}

				bPlayedMontage = true;
			}
		}
		else
		{
			ABILITY_LOG(Warning, TEXT("UPlayMontageAndWaitWithChildren call to PlayMontage failed!"));
		}
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("UPlayMontageAndWaitWithChildren called on invalid AbilitySystemComponent"));
	}

	if (!bPlayedMontage)
	{
		ABILITY_LOG(Warning, TEXT("UPlayMontageAndWaitWithChildren called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay),*InstanceName.ToString());
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast();
		}
	}

	SetWaitingOnAvatar();
}

void UPlayMontageAndWaitWithChildren::ExternalCancel()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCancelled.Broadcast();
	}
	Super::ExternalCancel();
}

void UPlayMontageAndWaitWithChildren::OnDestroy(bool AbilityEnded)
{
	// Note: Clearing montage end delegate isn't necessary since its not a multicast and will be cleared when the next montage plays.
	// (If we are destroyed, it will detect this and not do anything)

	// This delegate, however, should be cleared as it is a multicast
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(InterruptedHandle);
		if (AbilityEnded && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}

	Super::OnDestroy(AbilityEnded);

}

bool UPlayMontageAndWaitWithChildren::StopPlayingMontage()
{
	if (Ability == nullptr)
	{
		return false;
	}

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (ActorInfo == nullptr)
	{
		return false;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	// Check if the montage is still playing
	// The ability would have been interrupted, in which case we should automatically stop the montage
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && Ability)
	{
		if (ASC->GetAnimatingAbility() == Ability
			&& ASC->GetCurrentMontage() == MontageToPlay)
		{
			// Unbind delegates so they don't get called as well
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}
			
			ASC->CurrentMontageStop();
			return true;
		}
	}

	return false;
}

void UPlayMontageAndWaitWithChildren::PlayMontageWithSubSkeletalMesh() const
{
	TArray<UAnimInstance*> SubAnimInstances;
	GetChildrenAnimInstance(SubAnimInstances);
	for (UAnimInstance* SubComponentAnimIns : SubAnimInstances)
	{
		if (SubComponentAnimIns->Montage_Play(MontageToPlay,Rate,EMontagePlayReturnType::MontageLength,StartTimeSeconds) > 0 )
		{
			if (StartSection != NAME_None)
			{
				SubComponentAnimIns->Montage_JumpToSection(StartSection, MontageToPlay);
			}
		}
	}
}

void UPlayMontageAndWaitWithChildren::GetChildrenAnimInstance(TArray<UAnimInstance*>& AnimInstances) const 
{
	AnimInstances.Reset();
	TArray<USceneComponent*> SceneComponents;
	Ability->GetActorInfo().SkeletalMeshComponent->GetChildrenComponents(false,SceneComponents);
	for (USceneComponent* SceneComponent : SceneComponents)
	{
		USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(SceneComponent);
		if (!SkeletalMeshComponent || !SkeletalMeshComponent->GetAnimInstance())	continue;
		AnimInstances.Add(SkeletalMeshComponent->GetAnimInstance());
	}
}

void UPlayMontageAndWaitWithChildren::MontageStopWithChildren(float OverrideBlendOutTime)
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!ASC) return;
	UAnimMontage* CurrentMontage  = ASC->GetCurrentMontage();
	if (!CurrentMontage) return;
	TArray<UAnimInstance*> SubAnimInstances;
	GetChildrenAnimInstance(SubAnimInstances);
	for (UAnimInstance* SubComponentAnimIns : SubAnimInstances)
	{
		if (!SubComponentAnimIns->Montage_GetIsStopped(CurrentMontage))	
		SubComponentAnimIns->Montage_Stop(OverrideBlendOutTime >= 0.0f ? OverrideBlendOutTime : CurrentMontage->BlendOut.GetBlendTime());
	}
}


FString UPlayMontageAndWaitWithChildren::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? ToRawPtr(MontageToPlay) : AnimInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(TEXT("PlayMontageAndWait. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}

