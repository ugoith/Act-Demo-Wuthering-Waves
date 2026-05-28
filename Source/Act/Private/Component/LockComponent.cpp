// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/LockComponent.h"

#include "KismetTraceUtils.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
ULockComponent::ULockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


// Called every frame
void ULockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void ULockComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,TargetLocked,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME(ThisClass,TargetLockedHasSet);
	DOREPLIFETIME(ThisClass,OwnerCharacter);
	
}

void ULockComponent::OnRep_TargetLocked_Implementation(const AActor* OldTarget)
{
	
}

