// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Controller/WidgetController.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Act_ListenAttributeChange.generated.h"

struct FOnAttributeChangeData;
struct FGameplayAttribute;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS(BlueprintType,meta = (ExposedAsyncProxy = AsyncTask))
class ACT_API UAct_ListenAttributeChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true"))
	static UAct_ListenAttributeChange* ListenAttributeChange(UAbilitySystemComponent* ASC,FGameplayAttribute Attribute);

	UFUNCTION(BlueprintCallable)
	void EndTask();
	
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	FGameplayAttribute AttributeToListenFor;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnAttributeChangedSignature;
	
	void BindAttributeChangeCallback( const FOnAttributeChangeData& ChangedData);
	
};
