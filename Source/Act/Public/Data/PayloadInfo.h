// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Component/HitAndDamageComponent.h"
#include "UObject/Object.h"
#include "PayloadInfo.generated.h"

struct FApplyGEInfo;
class UGameplayEffect;
/**
 * 
 */
UCLASS(BlueprintType)
class ACT_API UPayloadInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<FApplyGEInfo> ApplyGEInfo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPhysicConfig PhysicConfig = FPhysicConfig();
 };
