// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ApplyGEToSelf.generated.h"

class UGameplayEffect;
/**
 * 
 */

UCLASS(DisplayName= "ApplyGEToSelf")
class ACT_API UAnimNotify_ApplyGEToSelf : public UAnimNotify
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Configuration")
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Configuration")
	float Magnitude;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Configuration")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTagQuery Query;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
