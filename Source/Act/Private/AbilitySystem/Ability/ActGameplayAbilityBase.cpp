// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/ActGameplayAbilityBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BlueprintGameplayTagLibrary.h"
#include "BlueprintFunctionLibrary/ActBlueprintFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

ACharacter* UActGameplayAbilityBase::GetAndCastAbilityOwnerCharacter(ECastResult& CastResult)
{
	CastResult = ECastResult::CastFailed;
	if (AbilityOwnerCharacter)
	{	
		CastResult = ECastResult::CastSucceeded;
		return AbilityOwnerCharacter;
	}
	
	AbilityOwnerCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (AbilityOwnerCharacter)
	{
		CastResult = ECastResult::CastSucceeded;
		return AbilityOwnerCharacter;	
	}
	return nullptr;
	
}

UActorComponent* UActGameplayAbilityBase::GetAvatarActorComponentByClass(TSubclassOf<UActorComponent> ComponentClass)
{
	if (AActor* AvatarActor = GetAvatarActorFromActorInfo())
	{
		return AvatarActor->GetComponentByClass(ComponentClass);
	}
	return nullptr;
}

void UActGameplayAbilityBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,AbilityOwnerCharacter);
	DOREPLIFETIME(ThisClass,QueryOwnedGameplayTags);
	DOREPLIFETIME(ThisClass,AttributeInspector);
}

void UActGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	/*if (IsEndAbilityValid(Handle, ActorInfo))
	{
		FGameplayEventData PayloadData;
		PayloadData.OptionalObject = this;
		PayloadData.EventTag = EventTags::EventTags_Ability_End;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(),EventTags::EventTags_Ability_End,PayloadData);;
	}*/
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}

bool UActGameplayAbilityBase::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent,
                                                                const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
                                                                FGameplayTagContainer* OptionalRelevantTags) const
{
	FGameplayTagContainer OwnedGameplayTags;
	AbilitySystemComponent.GetOwnedGameplayTags(OwnedGameplayTags);
	
	bool bQueryResult = UBlueprintGameplayTagLibrary::DoesContainerMatchTagQuery(OwnedGameplayTags,QueryOwnedGameplayTags);
	bQueryResult = bQueryResult or UBlueprintGameplayTagLibrary::IsTagQueryEmpty(QueryOwnedGameplayTags) ;
	if (bDebugQueryResult) 
		GEngine->AddOnScreenDebugMessage(0,2.f,FColor::Red,
			FString::Printf(TEXT("QueryOwnedGameplayTagsResult : %hs"),bQueryResult ? "true" : "false"));
	
	if (!bQueryResult)
	{
		return false;
	}
	
	/*const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{*/
		for (const FAttributeInspector& Inspector : AttributeInspector)
		{
			if (!UActBlueprintFunctionLibrary::IsSatisfyAttributeInspector(&AbilitySystemComponent, Inspector)) return false;
		}
	//}
	
	return Super::DoesAbilitySatisfyTagRequirements(AbilitySystemComponent, SourceTags, TargetTags,OptionalRelevantTags);
}

bool UActGameplayAbilityBase::QueryAvatarActorOwnedTags(FGameplayTagQuery Query)
{
	FGameplayTagContainer OwnedGameplayTags;
	GetAvatarActorOwnedGameplayTags(OwnedGameplayTags);
	return UBlueprintGameplayTagLibrary::IsTagQueryEmpty(Query) || UBlueprintGameplayTagLibrary::DoesContainerMatchTagQuery(OwnedGameplayTags,Query);
}

bool UActGameplayAbilityBase::TryActivateAvatarAbilityByClass(TSubclassOf<UGameplayAbility> AbilityToActivate,bool bAllowRemoteActivation)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent) return false;
	return AbilitySystemComponent->TryActivateAbilityByClass(AbilityToActivate,bAllowRemoteActivation);
}

bool UActGameplayAbilityBase::TryActivateAvatarAbilityByTag(const FGameplayTagContainer& GameplayTagContainer,
	bool bAllowRemoteActivation)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent) return false;
	return AbilitySystemComponent->TryActivateAbilitiesByTag(GameplayTagContainer,bAllowRemoteActivation);
}

bool UActGameplayAbilityBase::VerifyLastInputValidate(FVector& LastInputVector)
{
	LastInputVector = FVector();
	
	if (UCharacterMovementComponent* MovementComp = GetAvatarActorMovementComponent())
	{
		LastInputVector = MovementComp->GetLastInputVector();
	}
	
	return !LastInputVector.IsNearlyZero();
}


