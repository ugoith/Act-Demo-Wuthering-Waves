// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "AppearanceController.generated.h"
class UAbilityTask;
//一个Tag的Add或添加对应一个外观配置选项
USTRUCT(BlueprintType)
struct FAppearanceChangeConfigWhenTagChange
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag = FGameplayTag();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTriggerOnce = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (Tooltip = "为false时，则监听标签移除事件"))
	bool bOnAdded = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag,bool> MeshesVisibilityConfig;
};

UCLASS(ClassGroup=(AppearanceComponent),BlueprintType,Blueprintable, meta=(BlueprintSpawnableComponent))
class ACT_API UAppearanceController : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAppearanceController();
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="OwnerInfo")
	TMap<FGameplayTag,USkeletalMeshComponent*> OwnerSubMeshesInfo;
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void Deactivate() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Config|AppearanceChangeConfig")
	TArray<FAppearanceChangeConfigWhenTagChange> AppearanceChangeConfigWhenTagChanges;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void GetOwnerSubMeshes();
	
	void ReceiveOnGameplayTagChanged(const FGameplayTag Tag, int32 NewCount);
private:
	FDelegateHandle RegisterGenericGameplayTagChangedDelegateHandle;
};
