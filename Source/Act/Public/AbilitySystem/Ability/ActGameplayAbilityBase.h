// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BlueprintGameplayTagLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActGameplayAbilityBase.generated.h"

enum class ECompareOperationEnum : uint8;
class UBlueprintGameplayTagLibrary;

UENUM(BlueprintType)
enum class ECastResult: uint8
{
	CastSucceeded,
	CastFailed
};

UENUM(BlueprintType)
enum class EComparerOperation : uint8
{
	Equal	UMETA(DisplayName = "=="),
	NotEqual UMETA(DisplayName = "!="),
	LessThan UMETA(DisplayName = "<"),
	LessThanOrEqual UMETA(DisplayName = "<="),
	GreaterThan UMETA(DisplayName = ">"),
	GreaterThanOrEqual	UMETA(DisplayName = ">="),
};

USTRUCT(BlueprintType)
struct FAttributeInspector
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute Attribute;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EComparerOperation Operation = EComparerOperation::Equal;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float Value = 0.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayAttribute OtherAttribute;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bUseOtherAttribute = false;
};

/**
 * 
 */
UCLASS()
class ACT_API UActGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Replicated)
	TObjectPtr<ACharacter> AbilityOwnerCharacter;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated)
	FGameplayTagQuery QueryOwnedGameplayTags;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated)
	TArray<FAttributeInspector> AttributeInspector;
	
	
protected:
	UFUNCTION(BlueprintCallable,meta = (ExpandEnumAsExecs = "CastResult"))
	ACharacter* GetAndCastAbilityOwnerCharacter(ECastResult& CastResult);
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bDebugQueryResult = false;
public:
	UFUNCTION(BlueprintCallable,BlueprintPure,meta = (ComponentClass = "/Script/Engine.ActorComponent",
		DeterminesOutputType = "ComponentClass"))
	UActorComponent* GetAvatarActorComponentByClass(TSubclassOf<UActorComponent> ComponentClass);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE void GetAvatarActorOwnedGameplayTags(FGameplayTagContainer& TagContainer)
	{
		GetAbilitySystemComponentFromActorInfo_Checked()->GetOwnedGameplayTags(TagContainer);
	}
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool  QueryAvatarActorOwnedTags(FGameplayTagQuery Query);
	
protected:
	
	UFUNCTION(BlueprintCallable)
	bool TryActivateAvatarAbilityByClass(TSubclassOf<UGameplayAbility> AbilityToActivate,bool bAllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable)
	bool TryActivateAvatarAbilityByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE UCharacterMovementComponent* GetAvatarActorMovementComponent()
	{
		return Cast<UCharacterMovementComponent>(GetAvatarActorComponentByClass(UCharacterMovementComponent::StaticClass()));
	}
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool VerifyLastInputValidate(FVector& LastInputVector);
	
};