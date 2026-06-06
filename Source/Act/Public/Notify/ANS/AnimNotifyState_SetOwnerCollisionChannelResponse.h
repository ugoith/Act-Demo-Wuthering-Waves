// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SetOwnerCollisionChannelResponse.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "SetOwnerCollisionChannelRespones"))
class ACT_API UAnimNotifyState_SetOwnerCollisionChannelResponse : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_SetOwnerCollisionChannelResponse();
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
protected:
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<ECollisionChannel>,TEnumAsByte<ECollisionResponse>> StartCollisionConfig;	
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<ECollisionChannel>,TEnumAsByte<ECollisionResponse>> EndCollisionConfig;
	
	void SetOwnerCharacterCollisionChannel(const UActorComponent* MeshComponent,const TMap<TEnumAsByte<ECollisionChannel>,TEnumAsByte<ECollisionResponse>>& ChannelResponses);
};
