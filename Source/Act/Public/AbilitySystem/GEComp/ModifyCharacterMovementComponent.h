// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ModifyCharacterMovementComponent.generated.h"

/**
 * 修改角色移动属性的GameplayEffect组件
 * 通过OnGameplayEffectApplied回调来修改角色的移动属性
 */
UCLASS()
class ACT_API UModifyCharacterMovementComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	UModifyCharacterMovementComponent();

	// 移动速度修改
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0", UIMin = "0", UIMax = "2000"))
	float MaxWalkSpeedModifier = 600.0f;

	// 最大加速度修改
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0", UIMin = "0", UIMax = "5000"))
	float MaxAccelerationModifier = 2048.0f;

	// 减速度修改
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0", UIMin = "0", UIMax = "2000"))
	float BrakingDecelerationModifier = 2048.0f;

	// 刹车摩擦系数修改
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0", UIMin = "0", UIMax = "100"))
	float BrakingFrictionModifier = 2.0f;

	// 地面摩擦修改
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0", UIMin = "0", UIMax = "100"))
	float GroundFrictionModifier = 8.0f;

	// 空中控制能力修改
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0", UIMin = "0", UIMax = "1"))
	float AirControlModifier = 0.3f;

	// 跳跃高度修改
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0", UIMin = "0", UIMax = "2000"))
	float JumpVelocityModifier = 420.0f;

	// 旋转速率修改
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (UIMin = "0", UIMax = "3600"))
	float RotationRateModifier = 720.0f;

	// 是否自动朝向移动方向
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bOrientRotationToMovement = true;

	// 是否使用控制器期望的旋转
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bUseControllerDesiredRotation = false;

	// 是否保持水平地面速度
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bMaintainHorizontalGroundVelocity = true;

	// 是否可以蹲下
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bCanCrouch = true;

protected:
	// 当GameplayEffect被应用时调用
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

	// 当GameplayEffect被执行时调用（即时效果）
	virtual void OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

private:
	// 应用移动属性修改到目标角色
	void ApplyMovementModifierToCharacter(ACharacter* Character) const;

	// 恢复移动属性到原始值
	void RestoreMovementModifierToCharacter(ACharacter* Character) const;

	// 保存角色的原始值
	void SaveOriginalValuesToCharacter(ACharacter* Character) const;

	// 获取或创建一个临时组件来存储原始值
	mutable TMap<TWeakObjectPtr<ACharacter>, TMap<FName, float>> OriginalFloatValuesMap;
	mutable TMap<TWeakObjectPtr<ACharacter>, TMap<FName, bool>> OriginalBoolValuesMap;
	mutable TMap<TWeakObjectPtr<ACharacter>, TMap<FName, uint8>> OriginalEnumValuesMap;
};