// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/ANS/AnimNotifyState_HitTest.h"
#include "Engine/Engine.h"
#include "BlueprintFunctionLibrary/ActBlueprintFunctionLibrary.h"
#include "Component/HitAndDamageComponent.h"

void UAnimNotifyState_HitTest::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UHitAndDamageComponent* HDC = Cast<UHitAndDamageComponent>(
		UActBlueprintFunctionLibrary::GetOwnerComponentByClass(MeshComp,UHitAndDamageComponent::StaticClass()));
	if (!HDC) return;
	HDC->StartHitTest();
	
}

void UAnimNotifyState_HitTest::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (HitTestConfig.bDrawPreview)
	{
#if WITH_EDITOR
		UActBlueprintFunctionLibrary::DebugFromShapeType(MeshComp, HitTestConfig, 0.0334f);
#endif
	}
	
	UHitAndDamageComponent* HDC = Cast<UHitAndDamageComponent>(
		UActBlueprintFunctionLibrary::GetOwnerComponentByClass(MeshComp,UHitAndDamageComponent::StaticClass()));
	if (!HDC) return;
	HDC->UpdateHitTest(HitTestConfig);
	
}

void UAnimNotifyState_HitTest::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	UHitAndDamageComponent* HDC = Cast<UHitAndDamageComponent>(
		UActBlueprintFunctionLibrary::GetOwnerComponentByClass(MeshComp,UHitAndDamageComponent::StaticClass()));
	if (!HDC) return;
	HDC->EndHitTest();
	
}
