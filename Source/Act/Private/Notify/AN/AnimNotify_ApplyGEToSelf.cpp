// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AN/AnimNotify_ApplyGEToSelf.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BlueprintFunctionLibrary/ActBlueprintFunctionLibrary.h"

void UAnimNotify_ApplyGEToSelf::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       const FAnimNotifyEventReference& EventReference)
{
	UAbilitySystemComponent* ASC = UActBlueprintFunctionLibrary::GetOwnerAbililtySystemComponent(MeshComp);
	if (!ASC)
	{
		UE_LOG(LogTemp,Error,TEXT("Mesh Owner's AbilitySystemComp Is Not Valid ! ! !"))
		Super::Notify(MeshComp, Animation, EventReference);
		return;
	}
	if (!UActBlueprintFunctionLibrary::QueryTargetActorOwnedTags(MeshComp->GetOwner(),Query))
	{
		UE_LOG(LogTemp,Error,TEXT("Not Satisfy Tag Query ! ! !"))
		Super::Notify(MeshComp, Animation, EventReference);
		return;
	}
	FGameplayEffectContextHandle GECH = ASC->MakeEffectContext();
	GECH.AddSourceObject(MeshComp->GetOwner());
	FGameplayEffectSpecHandle GESH = FGameplayEffectSpecHandle();
	GESH = ASC->MakeOutgoingSpec(GameplayEffectClass,0.f,GECH);
	if (Tag.IsValid())
	GESH = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GESH,Tag,Magnitude);
	ASC->ApplyGameplayEffectSpecToSelf(*GESH.Data.Get());
	Super::Notify(MeshComp, Animation, EventReference);
}
