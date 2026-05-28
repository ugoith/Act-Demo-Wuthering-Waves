// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActBlueprintFunctionLibrary.generated.h"

class UActAttributeSet;
struct FAttributeInspector;
class FOnAttributeChangedSignature;
struct FHitTestConfig;
struct FGameplayTag;
class UAbilitySystemComponent;

/**
 *
 */
UCLASS()
class ACT_API UActBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "BFL_Act", meta = (ComponentClass = "/Script/Engine.ActorComponent",
		DefaultToSelf="Target"),
		meta = (DeterminesOutputType = "ComponentClass"))
	static UActorComponent* GetOwnerComponentByClass(UActorComponent* Target,TSubclassOf<UActorComponent> ComponentClass);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="BFL_Act",meta = (DefaultToSelf = "Target"))
	static UAbilitySystemComponent* GetOwnerAbililtySystemComponent(UActorComponent* Target);
	
	UFUNCTION(BlueprintCallable,Category = "BFL_Act",meta = (DefaultToSelf = "Target"))
	static ACharacter* GetOwnerCharacter(UActorComponent* Target);
	
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "BFL_Act")
	static void GetChildrenAnimInstancesByASC(UAbilitySystemComponent* ASC,TArray<UAnimInstance*>& AnimInstances);
	
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "BFL_Act",meta = (DefaultToSelf = "Target"))
	static bool QueryTargetActorOwnedTags(AActor* Target ,FGameplayTagQuery Query);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "BFL_Act",meta = (Tooltip = "若检查条件为空，则返回true"))
	static bool QueryTags(FGameplayTagContainer TagsToQuery ,FGameplayTagQuery Query);
	//Math
public:
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "BFL_Act")
	static FVector CalculateActorToTargetActorVector(AActor* From , AActor* To);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "BFL_Act")
	static void GetThreeDirection(FRotator InRot,FVector& Forward,FVector& Right,FVector& Up);

	UFUNCTION(BlueprintCallable,Category = "BFL_Act")
	static void DebugFromShapeType(USkeletalMeshComponent* MeshComp,FHitTestConfig HitTestConfig,float DeltaSeconds = 0.1f);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "BFL_Act")
	static bool IsSatisfyAttributeInspector(const UAbilitySystemComponent* AbilitySystemComponent, const FAttributeInspector& AttributeInspector);
	
public:
	UFUNCTION(BlueprintPure, CustomThunk, 
	meta = (
		DisplayName = "Contains All Elements", 
		Category = "Utilities|Array", 
		ArrayParm = "TargetArray,ItemsToFind",          // 声明这两个参数是通配符数组
		ArrayTypeDependentParams = "ItemsToFind",      // 强制 ItemsToFind 的类型跟随 TargetArray
		CompactNodeTitle = "CONTAINS ALL"
	))
	static bool Array_ContainsAll(const TArray<int32>& TargetArray, const TArray<int32>& ItemsToFind);

	// C++ 编译器的底层实现声明
	DECLARE_FUNCTION(execArray_ContainsAll);

};