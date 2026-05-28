// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ActPlayerState.generated.h"

class UActAbilitySystemComponent;
class UActAttributeSet;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class ACT_API AActPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AActPlayerState();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitySystem");
	TObjectPtr<UActAttributeSet> AttributeSet;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
