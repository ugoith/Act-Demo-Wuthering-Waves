// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Interface/AppearanceInterface.h"
#include "ActCharacterBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitySystemCompHasSet,UAbilitySystemComponent*);

class UGameplayEffect;
class UGameplayAbility;
class UMotionWarpingComponent;
class UActUserWidget;



UCLASS()
class ACT_API AActCharacterBase : public ACharacter,public IAbilitySystemInterface,public IAppearanceInterface
	,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActCharacterBase();

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Replicated)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated)
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated)
	TArray<TSubclassOf<UGameplayEffect>> InitializeEffects;
	
	FAbilitySystemCompHasSet AbilitySystemCompHasSet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Replicated,meta = (Tooltip = "0,玩家阵营;1,中立阵营;2,AI阵营"))
	uint8 TeamID;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual auto GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const -> void override;
	virtual void InitializeEffectsToSelf();
	virtual void InitAbiltityActorInfo();
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
};
