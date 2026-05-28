// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InputInfoManager.generated.h"

USTRUCT(BlueprintType)
struct FTriggerThreshold
{
	GENERATED_BODY()
	
	FTriggerThreshold():InputEventTag(FGameplayTag()),Threshold(0.f),TimePassed(0.f),AddTagsWhenExceedThreshold(FGameplayTag()){}
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTag InputEventTag;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Threshold;

	float TimePassed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTagContainer AddTagsWhenExceedThreshold;
};


UCLASS(ClassGroup=(Custom),Blueprintable,BlueprintType, meta=(BlueprintSpawnableComponent))
class ACT_API UInputInfoManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInputInfoManager();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<FGameplayTag,bool> PreInputTagConsumed;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated)
	FGameplayTag PreInputTag;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated)
	bool bCanPreInput;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FTriggerThreshold> TriggerThresholds;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintCallable)
	void UpdatePreInput(FGameplayTag TagToSet);
	UFUNCTION(BlueprintCallable)
	void SendPreInputEvent();
	
	UFUNCTION(BlueprintCallable)
	void StartPreInput();

	UFUNCTION(BlueprintCallable)
	void ResetAllParams();
	
	UFUNCTION(BlueprintCallable)
	void UpdateTriggerThresholds(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void ResetTriggerThresholds();
	
	FORCEINLINE FGameplayTagContainer& GetHasAddedTagsWhenExceedThreshold()
	{
		return HasAddedTagsWhenExceedThreshold;
	}
private:
	FGameplayTagContainer HasAddedTagsWhenExceedThreshold = FGameplayTagContainer();
};
