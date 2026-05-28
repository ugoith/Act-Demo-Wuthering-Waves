// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TargetActor/ActGameplayAbilityTargetActor.h"

#include "Abilities/GameplayAbility.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AActGameplayAbilityTargetActor::AActGameplayAbilityTargetActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AActGameplayAbilityTargetActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AActGameplayAbilityTargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActGameplayAbilityTargetActor::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	if (APlayerController* PC = Ability->GetActorInfo().PlayerController.Get())
	{
		PrimaryPC = PC;
	}
	SourceActor = Ability->GetAvatarActorFromActorInfo();


}

void AActGameplayAbilityTargetActor::ConfirmTargetingAndContinue()
{
	FVector QueryStartLocation = SourceActor ? SourceActor->GetActorLocation() : FVector(0, 0, 0);
	TArray<TWeakObjectPtr<AActor>> HitActors;
	TArray<FHitResult> HitResults;

	FCollisionQueryParams QueryParams;
	if (SourceActor)
		QueryParams.AddIgnoredActor(SourceActor);
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;
	bool bIsHit = GetWorld()->SweepMultiByChannel(HitResults,QueryStartLocation,QueryStartLocation,FQuat()
		,ECC_Visibility,FCollisionShape::MakeSphere(Radius),QueryParams);
	if (bIsHit)
	{
		for (const FHitResult& HitResult:HitResults)
		{

				for (const TSubclassOf<AActor>& QueryClass :TargetClasses)
				{
					AActor* HitActor = HitResult.GetActor();
					if (HitActor->IsA(QueryClass))
						HitActors.AddUnique(HitActor);
				}
		}
	}

	if (HitActors.Num()==0)
	{
		check(ShouldProduceTargetData());
		if (IsConfirmTargetingAllowed())
		{
			TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
		}
	}
	else
	{
		check(ShouldProduceTargetData());
		if (IsConfirmTargetingAllowed())
		{
			FGameplayAbilityTargetDataHandle TargetDateHandle = StartLocation.MakeTargetDataHandleFromActors(HitActors);
			TargetDataReadyDelegate.Broadcast(TargetDateHandle);
		}
	}
}

void AActGameplayAbilityTargetActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass,Radius);
	DOREPLIFETIME(ThisClass,TargetClasses);

}
