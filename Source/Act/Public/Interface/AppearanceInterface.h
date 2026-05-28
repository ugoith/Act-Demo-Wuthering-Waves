// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AppearanceInterface.generated.h"

struct FGameplayTag;
// This class does not need to be modified.
UINTERFACE()
class UAppearanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACT_API IAppearanceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable,Category="AppearanceInterface")
	void GetSubMeshes(TMap<FGameplayTag,USkeletalMeshComponent*>& SubMeshesInfo);
};
