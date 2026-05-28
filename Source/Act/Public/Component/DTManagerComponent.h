// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DTManagerComponent.generated.h"
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FActionConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (Tooltip = "给行为附加Tag，当前蒙太奇结束时去除"))
	FGameplayTag ActionTag;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (Tooltip = "激活行为时，自身需要的Tag条件"))
	FGameplayTagQuery QueryOwnedTag;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (Tooltip = "当条件满足时，取最大优先级的Action"))
	uint8 Priority = 0;
	
	/*UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (Tooptip = "这个能力使用时同步使用的能力，在蒙太奇逻辑执行前,设置角色旋转等"))
	TArray<TSubclassOf<UGameplayAbility>> ActivateOtherAbilitiesSimultaneously;*/
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta = (Tooltip = "注释"))
	FString Describe;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "MontageConfig")
	TObjectPtr<UAnimMontage> Montage;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "MontageConfig")
	FName StartSection;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "MontageConfig")
	float StartTime = 0.f;
};

USTRUCT(BlueprintType)
struct FNPCActionConfig : public FTableRowBase
{
	GENERATED_BODY()
	
};



UCLASS(ClassGroup=(DataTable), Blueprintable,BlueprintType,meta=(BlueprintSpawnableComponent))
class ACT_API UDTManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDTManagerComponent();
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (RowType = "FActionConfig"))
	TObjectPtr<UDataTable> ActionData;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
