// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEComp/ModifyCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

UModifyCharacterMovementComponent::UModifyCharacterMovementComponent()
{
}

void UModifyCharacterMovementComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);

	// 从GESpec获取源角色（应用效果的actor）
	AActor* SourceActor = GESpec.GetContext().GetEffectCauser();
	if (SourceActor)
	{
		// 尝试从源actor获取AbilitySystemComponent，然后获取avatar
		UAbilitySystemComponent* ASC = SourceActor->FindComponentByClass<UAbilitySystemComponent>();
		if (ASC)
		{
			AActor* TargetActor = ASC->GetAvatarActor();
			if (TargetActor && TargetActor->IsA(ACharacter::StaticClass()))
			{
				ACharacter* Character = Cast<ACharacter>(TargetActor);

				// 保存原始值（如果还没有保存）
				TWeakObjectPtr<ACharacter> WeakChar(Character);
				if (!OriginalFloatValuesMap.Contains(WeakChar))
				{
					SaveOriginalValuesToCharacter(Character);
				}

				// 应用移动修改
				ApplyMovementModifierToCharacter(Character);

				UE_LOG(LogTemp, Log, TEXT("ModifyCharacterMovementComponent: 已应用移动修改到角色 %s"), *Character->GetName());
			}
		}
	}
}

void UModifyCharacterMovementComponent::OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectExecuted(ActiveGEContainer, GESpec, PredictionKey);

	// 对于即时效果，我们也需要恢复原始值
	AActor* SourceActor = GESpec.GetContext().GetEffectCauser();
	if (SourceActor)
	{
		UAbilitySystemComponent* ASC = SourceActor->FindComponentByClass<UAbilitySystemComponent>();
		if (ASC)
		{
			AActor* TargetActor = ASC->GetAvatarActor();
			if (TargetActor && TargetActor->IsA(ACharacter::StaticClass()))
			{
				ACharacter* Character = Cast<ACharacter>(TargetActor);
				RestoreMovementModifierToCharacter(Character);
			}
		}
	}
}

void UModifyCharacterMovementComponent::ApplyMovementModifierToCharacter(ACharacter* Character) const
{
	if (!Character) return;

	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (!MovementComponent) return;

	// 应用速度相关修改
	MovementComponent->MaxWalkSpeed = MaxWalkSpeedModifier;
	MovementComponent->MaxAcceleration = MaxAccelerationModifier;
	MovementComponent->BrakingDecelerationWalking = BrakingDecelerationModifier;
	MovementComponent->BrakingFrictionFactor = BrakingFrictionModifier;
	MovementComponent->GroundFriction = GroundFrictionModifier;
	MovementComponent->AirControl = AirControlModifier;
	MovementComponent->JumpZVelocity = JumpVelocityModifier;

	// 应用旋转相关修改
	MovementComponent->RotationRate = FRotator(0.0f, RotationRateModifier, 0.0f);

	// 应用行为设置
	MovementComponent->bOrientRotationToMovement = bOrientRotationToMovement;
	MovementComponent->bUseControllerDesiredRotation = bUseControllerDesiredRotation;
	MovementComponent->bMaintainHorizontalGroundVelocity = bMaintainHorizontalGroundVelocity;
	MovementComponent->NavAgentProps.bCanCrouch = bCanCrouch;
}

void UModifyCharacterMovementComponent::RestoreMovementModifierToCharacter(ACharacter* Character) const
{
	if (!Character) return;

	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (!MovementComponent) return;

	// 从保存的值中恢复
	TWeakObjectPtr<ACharacter> WeakChar(Character);
	if (OriginalFloatValuesMap.Contains(WeakChar))
	{
		const TMap<FName, float>& FloatValues = OriginalFloatValuesMap[WeakChar];
		if (FloatValues.Contains("MaxWalkSpeed"))
			MovementComponent->MaxWalkSpeed = FloatValues["MaxWalkSpeed"];
		if (FloatValues.Contains("MaxAcceleration"))
			MovementComponent->MaxAcceleration = FloatValues["MaxAcceleration"];
		if (FloatValues.Contains("BrakingDecelerationWalking"))
			MovementComponent->BrakingDecelerationWalking = FloatValues["BrakingDecelerationWalking"];
		if (FloatValues.Contains("BrakingFrictionFactor"))
			MovementComponent->BrakingFrictionFactor = FloatValues["BrakingFrictionFactor"];
		if (FloatValues.Contains("GroundFriction"))
			MovementComponent->GroundFriction = FloatValues["GroundFriction"];
		if (FloatValues.Contains("AirControl"))
			MovementComponent->AirControl = FloatValues["AirControl"];
		if (FloatValues.Contains("JumpZVelocity"))
			MovementComponent->JumpZVelocity = FloatValues["JumpZVelocity"];
	}

	if (OriginalBoolValuesMap.Contains(WeakChar))
	{
		const TMap<FName, bool>& BoolValues = OriginalBoolValuesMap[WeakChar];
		if (BoolValues.Contains("bOrientRotationToMovement"))
			MovementComponent->bOrientRotationToMovement = BoolValues["bOrientRotationToMovement"];
		if (BoolValues.Contains("bUseControllerDesiredRotation"))
			MovementComponent->bUseControllerDesiredRotation = BoolValues["bUseControllerDesiredRotation"];
		if (BoolValues.Contains("bMaintainHorizontalGroundVelocity"))
			MovementComponent->bMaintainHorizontalGroundVelocity = BoolValues["bMaintainHorizontalGroundVelocity"];
		if (BoolValues.Contains("NavAgentProps.bCanCrouch"))
			MovementComponent->NavAgentProps.bCanCrouch = BoolValues["NavAgentProps.bCanCrouch"];
	}

	// 从映射中移除该角色的保存值
	OriginalFloatValuesMap.Remove(WeakChar);
	OriginalBoolValuesMap.Remove(WeakChar);
	OriginalEnumValuesMap.Remove(WeakChar);
}

void UModifyCharacterMovementComponent::SaveOriginalValuesToCharacter(ACharacter* Character) const
{
	if (!Character) return;

	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (!MovementComponent) return;

	TWeakObjectPtr<ACharacter> WeakChar(Character);

	// 保存浮点数属性
	TMap<FName, float>& FloatValues = OriginalFloatValuesMap.FindOrAdd(WeakChar);
	FloatValues.Add("MaxWalkSpeed", MovementComponent->MaxWalkSpeed);
	FloatValues.Add("MaxAcceleration", MovementComponent->MaxAcceleration);
	FloatValues.Add("BrakingDecelerationWalking", MovementComponent->BrakingDecelerationWalking);
	FloatValues.Add("BrakingFrictionFactor", MovementComponent->BrakingFrictionFactor);
	FloatValues.Add("GroundFriction", MovementComponent->GroundFriction);
	FloatValues.Add("AirControl", MovementComponent->AirControl);
	FloatValues.Add("JumpZVelocity", MovementComponent->JumpZVelocity);

	// 保存布尔值属性
	TMap<FName, bool>& BoolValues = OriginalBoolValuesMap.FindOrAdd(WeakChar);
	BoolValues.Add("bOrientRotationToMovement", MovementComponent->bOrientRotationToMovement);
	BoolValues.Add("bUseControllerDesiredRotation", MovementComponent->bUseControllerDesiredRotation);
	BoolValues.Add("bMaintainHorizontalGroundVelocity", MovementComponent->bMaintainHorizontalGroundVelocity);
	BoolValues.Add("NavAgentProps.bCanCrouch", MovementComponent->NavAgentProps.bCanCrouch);
}