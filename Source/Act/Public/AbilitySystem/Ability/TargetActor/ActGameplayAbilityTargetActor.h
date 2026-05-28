// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ActGameplayAbilityTargetActor.generated.h"

UCLASS()
class ACT_API AActGameplayAbilityTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActGameplayAbilityTargetActor();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated,meta = (ExposeOnSpawn = true))
	float Radius;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = true))
	TArray<TSubclassOf<AActor>> TargetClasses;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
