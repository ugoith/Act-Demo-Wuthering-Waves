// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameplayTags.h"
#include "Engine/DataAsset.h"
#include "ControllerInputConfig.generated.h"
class UGameplayAbility;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FTriggerEventNeedTriggerAbilities
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	ETriggerEvent TriggerEvent = ETriggerEvent::Started;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag PreInputTagToSend;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTagContainer AbilitiesTags;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToTrigger;
};

USTRUCT(BlueprintType)
struct FInputActionTriggerConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FTriggerEventNeedTriggerAbilities> TriggerConfig;
};

class UInputAction;
UCLASS(BlueprintType)
class ACT_API UControllerInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TMap<FGameplayTag,TObjectPtr<UInputAction>> InputActions;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TMap<FGameplayTag,FInputActionTriggerConfig> InputActionConfig;
};
