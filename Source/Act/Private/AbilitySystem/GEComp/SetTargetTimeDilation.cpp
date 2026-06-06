// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEComp/SetTargetTimeDilation.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "Engine/World.h"
#include "GameplayEffect.h"
#include "Misc/DataValidation.h"
#include "TimerManager.h"

/** 帧延迟保守折算（30fps下限），仅用于编辑器/运行时校验。实际执行用当前帧 DeltaSeconds × 帧数 计算等效秒数 */
static float EstimateFrameDelaySeconds(uint8 DelayFrames)
{
	return DelayFrames > 0 ? DelayFrames / 30.0f : 0.f;
}

/** 计算校验用的等效延迟秒数 */
static float GetEffectiveDelayForValidation(bool bUseDelayFrames, float DelayToTrigger, uint8 DelayFrames)
{
	if (bUseDelayFrames && DelayFrames > 0)
	{
		return EstimateFrameDelaySeconds(DelayFrames);
	}
	return FMath::Max(DelayToTrigger, 0.f);
}

bool USetTargetTimeDilation::CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer,
	const FGameplayEffectSpec& GESpec) const
{
	// Instant GE 永远不会触发 OnActiveGameplayEffectAdded，时间膨胀无从恢复
	if (GESpec.Def->DurationPolicy == EGameplayEffectDurationType::Instant)
	{
		ABILITY_LOG(Warning, TEXT("SetTargetTimeDilation: GE '%s' 是 Instant 类型，无法使用时间膨胀组件，已阻止应用。"), *GetNameSafe(GESpec.Def));
		return false;
	}

	// 延迟时间必须小于 GE 的持续时间（帧延迟按30fps保守估算）
	const float EffectiveDelay = GetEffectiveDelayForValidation(bUseDelayFrames, DelayToTrigger, DelayFrames);
	if (EffectiveDelay > 0.f)
	{
		const float GEDuration = GESpec.GetDuration();
		if (GEDuration > 0.f && EffectiveDelay >= GEDuration)
		{
			ABILITY_LOG(Warning, TEXT("SetTargetTimeDilation: GE '%s' 的延迟 (%.2fs) 大于等于持续时间 (%.2fs)，已阻止应用。"),
				*GetNameSafe(GESpec.Def), EffectiveDelay, GEDuration);
			return false;
		}
	}

	return true;
}

bool USetTargetTimeDilation::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const
{
	UAbilitySystemComponent* ASC = ActiveGEContainer.Owner;
	if (!ensure(ASC))
	{
		return false;
	}

	AActor* TargetActor = ASC->GetAvatarActor();
	if (!TargetActor)
	{
		return true;
	}

	const float OriginalTimeDilation = TargetActor->CustomTimeDilation;
	TSharedPtr<bool> bDilationApplied = MakeShared<bool>(false);
	TSharedPtr<FTimerHandle> TimerHandle = MakeShared<FTimerHandle>();

	TWeakObjectPtr<AActor> WeakActor(TargetActor);
	const float DilationValue = TargetTimeDilation;

	if (bUseDelayFrames && DelayFrames > 0)
	{
		// 帧延迟：用当前帧 DeltaSeconds × 帧数 折算为秒，one-shot Timer
		if (UWorld* World = ASC->GetWorld())
		{
			const float CurrentDelta = FMath::Max(World->GetDeltaSeconds(), 1.f / 120.f);
			World->GetTimerManager().SetTimer(*TimerHandle,
				[WeakActor, bDilationApplied, DilationValue]()
				{
					if (AActor* Actor = WeakActor.Get())
					{
						Actor->CustomTimeDilation = DilationValue;
						*bDilationApplied = true;
					}
				},
				DelayFrames * CurrentDelta, false);
		}
	}
	else if (DelayToTrigger > 0.f)
	{
		// 秒延迟：World Timer 一次性触发
		if (UWorld* World = ASC->GetWorld())
		{
			World->GetTimerManager().SetTimer(*TimerHandle,
				[WeakActor, bDilationApplied, DilationValue]()
				{
					if (AActor* Actor = WeakActor.Get())
					{
						Actor->CustomTimeDilation = DilationValue;
						*bDilationApplied = true;
					}
				},
				DelayToTrigger, false);
		}
	}
	else
	{
		// 无延迟，立即应用
		TargetActor->CustomTimeDilation = TargetTimeDilation;
		*bDilationApplied = true;
	}

	// 绑定移除事件，通过payload传递所有上下文（不在 Component 上存状态）
	FActiveGameplayEffectEvents* EventSet = ASC->GetActiveEffectEventSet(ActiveGE.Handle);
	if (EventSet)
	{
		EventSet->OnEffectRemoved.AddUObject(
			this,
			&USetTargetTimeDilation::OnGameplayEffectRemoved,
			ASC,
			OriginalTimeDilation,
			bDilationApplied,
			TimerHandle
		);
	}

	return true;
}

void USetTargetTimeDilation::OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo,
	UAbilitySystemComponent* ASC, float OriginalTimeDilation, TSharedPtr<bool> bDilationApplied, TSharedPtr<FTimerHandle> TimerHandle) const
{
	// 取消尚未触发的延迟Timer（帧延迟和秒延迟共用 FTimerHandle）
	if (TimerHandle && TimerHandle->IsValid() && ASC)
	{
		if (UWorld* World = ASC->GetWorld())
		{
			World->GetTimerManager().ClearTimer(*TimerHandle);
		}
	}

	// 仅当膨胀已被实际应用后才恢复
	if (bDilationApplied && *bDilationApplied && ASC)
	{
		if (AActor* TargetActor = ASC->GetAvatarActor())
		{
			TargetActor->CustomTimeDilation = OriginalTimeDilation;
		}
	}
}

#if WITH_EDITOR
EDataValidationResult USetTargetTimeDilation::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	const UGameplayEffect* OwningGE = GetOwner();
	if (!OwningGE)
	{
		return Result;
	}

	// Instant 校验
	if (OwningGE->DurationPolicy == EGameplayEffectDurationType::Instant)
	{
		Context.AddError(FText::Format(
			FTextFormat::FromString(TEXT("GE '{0}' 是 Instant 类型，SetTargetTimeDilation 组件只在 HasDuration / Infinite 类型的 GE 上生效。")),
			FText::FromString(GetNameSafe(OwningGE))
		));
		Result = EDataValidationResult::Invalid;
	}

	// 延迟 < 持续时间 校验（帧延迟按30fps保守估算）
	const float EffectiveDelay = GetEffectiveDelayForValidation(bUseDelayFrames, DelayToTrigger, DelayFrames);
	if (EffectiveDelay > 0.f && OwningGE->DurationPolicy == EGameplayEffectDurationType::HasDuration)
	{
		float BaseDuration = 0.f;
		if (OwningGE->DurationMagnitude.GetStaticMagnitudeIfPossible(1.f, BaseDuration))
		{
			if (BaseDuration > 0.f && EffectiveDelay >= BaseDuration)
			{
				Context.AddError(FText::Format(
					FTextFormat::FromString(TEXT("GE '{0}' 的延迟 ({1}s, 帧延迟按30fps保守估算) 大于等于持续时间 ({2}s)，时间膨胀可能永远不会生效。")),
					FText::FromString(GetNameSafe(OwningGE)),
					FText::AsNumber(EffectiveDelay),
					FText::AsNumber(BaseDuration)
				));
				Result = EDataValidationResult::Invalid;
			}
		}
	}

	return Result;
}
#endif
