// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "CheckGameplayTagsOnActorASC.generated.h"
/**
 * 
 */
UCLASS()
class ACT_API UCheckGameplayTagsOnActorASC : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UCheckGameplayTagsOnActorASC(const FObjectInitializer& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	
	
protected:
	UPROPERTY(EditAnywhere, Category=GameplayTagCheck,
	Meta=(ToolTips="Which Actor (from the blackboard) should be checked for these gameplay tags?"))
	struct FBlackboardKeySelector ActorToCheck;

	UPROPERTY(EditAnywhere, Category=GameplayTagCheck)
	EGameplayContainerMatchType TagsToMatch;

	UPROPERTY(EditAnywhere, Category=GameplayTagCheck)
	FGameplayTagContainer GameplayTags;
	
	FDelegateHandle OnGameplayTagCountChanged;
	
	UAbilitySystemComponent* GetActorToCheckAbilitySystemComp(UBehaviorTreeComponent& OwnerComp);
	
	//virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
