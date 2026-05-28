// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SetTimeDilation.generated.h"

struct FGameplayTagQuery;
/**
 * 
 */
UENUM(BlueprintType)
enum class ESetTimeDilationType : uint8
{
	Self = 0,
	Global = 1,
	ExceptSelf = 2
};
	

UCLASS(DisplayName="SetTimeDilation")
class ACT_API UAnimNotifyState_SetTimeDilation : public UAnimNotifyState
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ESetTimeDilationType DilationType = ESetTimeDilationType::Self;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float TimeDilation = 0.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ScopeOfAffection = 2000.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bDebug = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTagQuery QueryOwnedTags = FGameplayTagQuery();
	
	float OriginalTimeDilation = 1.f;
	void SetTimeDilation(UActorComponent* ActorComponent);
	void ResetTimeDilation(UActorComponent* ActorComponent);
	FORCEINLINE TArray<AActor*> GetAffectedActors()
	{
		return AffectedActors;
	}
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
private:
	TArray<AActor*> AffectedActors;
};
