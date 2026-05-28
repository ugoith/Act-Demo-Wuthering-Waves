// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "ActAnimInstance.generated.h"

struct FGameplayTagContainer;
class UAbilitySystemComponent;
/**
 * 
 */
UENUM(BlueprintType,meta = (Tooltip = "角色停止移动的状态类型，区分步行/跑步/冲刺停止"))
enum class EStopMoveStates : uint8
{
	None UMETA(Tooltip = "无停止状态，默认初始值"),
	WalkStop UMETA(Tooltip = "步行停止状态"),
	RunStop UMETA(Tooltip = "跑步停止状态"),
	SprintStop  UMETA(Tooltip = "冲刺停止状态")
};

UCLASS()
class ACT_API UActAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	
	void SetOwnerAbilitySystemComponent(UAbilitySystemComponent* InASC);


protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	APawn* OwnerPawn;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> OwnerAbilitySystemComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FGameplayTagContainer OwnedGameplayTags;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	FORCEINLINE void UpdateOwnerGameplayTags(const FGameplayTag OperateTag, int32 NewCount)
	{
		if (NewCount == 0)
		{
			OwnedGameplayTags.RemoveTag(OperateTag);
		}
		else
		{
			OwnedGameplayTags.AddTag(OperateTag);
		}
		/*if (OwnerAbilitySystemComp)
		{
			OwnerAbilitySystemComp->GetOwnedGameplayTags(OwnedGameplayTags);
		}*/
	}
};
