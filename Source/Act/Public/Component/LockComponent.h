// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "LockComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetLockedHasSet,FGameplayTag,SetCondition);

UCLASS(ClassGroup=(Custom), BlueprintType,Blueprintable,meta=(BlueprintSpawnableComponent))
class ACT_API ULockComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULockComponent();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,ReplicatedUsing= OnRep_TargetLocked )
	TObjectPtr<AActor> TargetLocked;

	UPROPERTY(BlueprintAssignable,BlueprintCallable,Replicated)
	FTargetLockedHasSet TargetLockedHasSet;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated)
	TObjectPtr<ACharacter> OwnerCharacter;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnRep_TargetLocked(const AActor* OldTarget);
	
	
	
};
