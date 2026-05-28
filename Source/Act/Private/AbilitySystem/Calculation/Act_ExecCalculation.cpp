// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Calculation/Act_ExecCalculation.h"
#include "AbilitySystem/ActAttributeSet.h"

UAct_ExecCalculation::UAct_ExecCalculation()
{
	DEFINE_ATTRIBUTE_CAPTUREDEF(UActAttributeSet,Health,Source,false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UActAttributeSet,MaxHealth,Source,false);
	
	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(MaxHealthDef);
	
}

void UAct_ExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters RelevantParameters;
	RelevantParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	RelevantParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealthDef,RelevantParameters,Health);
	UE_LOG(LogTemp,Warning,TEXT("Health : %f"),Health);
	
	float FinalResult = 0.f;
	FinalResult = FMath::Max<float>(0.f,Health - 5.f);
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthDef.AttributeToCapture,EGameplayModOp::Override,FinalResult));
	UE_LOG(LogTemp,Warning,TEXT("FinalHealth : %f"),FinalResult);
}
