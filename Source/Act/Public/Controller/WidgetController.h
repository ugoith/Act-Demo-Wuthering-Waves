// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Game/ActPlayerController.h"
#include "UObject/Object.h"
#include "WidgetController.generated.h"

class AActPlayerState;
struct FGameplayEffectContextHandle;
struct FGameplayEffectSpecHandle;
class UAbilitySystemComponent;
class UActAttributeSet;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChangedSignature,
	FGameplayAttribute, Attribute,float, NewValue,float, OldValue);

UCLASS(Blueprintable)
class ACT_API UWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UWidgetController();
	
	void SetWidgetControllerParams(AActPlayerController* InPC, APawn* InPawn, AActPlayerState* InPS,
		UAbilitySystemComponent* InASC,UActAttributeSet* InAS);
	void BindCallbacks();
	void BroadcastInitialValues();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<AActPlayerController> PlayerController;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<APawn> PlayerPawn;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<AActPlayerState> PlayerState;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UActAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnAttributeChanged;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FGameplayAttribute> BindAttributes;
};
