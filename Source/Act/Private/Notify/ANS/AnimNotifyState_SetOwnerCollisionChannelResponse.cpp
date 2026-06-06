// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/ANS/AnimNotifyState_SetOwnerCollisionChannelResponse.h"

#include "BlueprintFunctionLibrary/ActBlueprintFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

UAnimNotifyState_SetOwnerCollisionChannelResponse::UAnimNotifyState_SetOwnerCollisionChannelResponse()
{
	StartCollisionConfig.Add(ECC_Pawn,ECR_Ignore);
	EndCollisionConfig.Add(ECC_Pawn,ECR_Block);
}

void UAnimNotifyState_SetOwnerCollisionChannelResponse::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                                                    UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	SetOwnerCharacterCollisionChannel(MeshComp,StartCollisionConfig);
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UAnimNotifyState_SetOwnerCollisionChannelResponse::NotifyEnd(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	SetOwnerCharacterCollisionChannel(MeshComp,EndCollisionConfig);
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UAnimNotifyState_SetOwnerCollisionChannelResponse::SetOwnerCharacterCollisionChannel(
	const UActorComponent* MeshComponent,const TMap<TEnumAsByte<ECollisionChannel>,TEnumAsByte<ECollisionResponse>>& ChannelResponses)
{
	ACharacter* Character = Cast<ACharacter>(MeshComponent->GetOwner());
	if (!Character)
	{
		return;
	}
	UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
	
	for (const TPair<ECollisionChannel,ECollisionResponse> CollisionChannelResponse : ChannelResponses)
	{
		CapsuleComponent->SetCollisionResponseToChannel(CollisionChannelResponse.Key, CollisionChannelResponse.Value);
	}
}
