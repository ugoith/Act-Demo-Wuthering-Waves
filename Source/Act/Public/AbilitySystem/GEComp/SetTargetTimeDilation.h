// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "SetTargetTimeDilation.generated.h"

struct FGameplayEffectRemovalInfo;
class UAbilitySystemComponent;

/**
 * 设置命中目标的时间膨胀（CustomTimeDilation）
 * 配合带持续时间的GameplayEffect使用：应用时修改目标时间膨胀，移除时恢复原始值
 */
UCLASS()
class ACT_API USetTargetTimeDilation : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	/** 检查GE是否可以应用到目标 */
	virtual bool CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const override;

	/** GE被添加到活跃容器时：保存原始时间膨胀、设置新值、绑定移除事件 */
	virtual bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const override;

#if WITH_EDITOR
	/** 编辑器校验：Instant 类型的 GE 不应使用此组件（需配合 HasDuration / Infinite 使用） */
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

public:
	/** 命中目标后的时间膨胀系数（1.0=正常速度，<1.0=慢动作，>1.0=加速） */
	UPROPERTY(EditDefaultsOnly, Category = TimeDilation)
	float TargetTimeDilation = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = TimeDilation)
	float DelayToTrigger = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = TimeDilation)
	bool bUseDelayFrames = true;
	
	UPROPERTY(EditDefaultsOnly, Category = TimeDilation)
	uint8 DelayFrames = 0;

	
private:
	/** GE被移除时：取消挂起的Timer，若已应用膨胀则恢复原始值 */
	void OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo, UAbilitySystemComponent* ASC, float OriginalTimeDilation, TSharedPtr<bool> bDilationApplied, TSharedPtr<FTimerHandle> TimerHandle) const;
};
