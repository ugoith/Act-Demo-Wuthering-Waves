// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/AIInterface.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "ActAIControllerBase.generated.h"

class UAbilitySystemComponent;

UCLASS()
class ACT_API AActAIControllerBase : public AAIController , public IAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActAIControllerBase();
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UBehaviorTree>	BehaviorTree;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	virtual bool ActivateControlledAbilities(TSubclassOf<UGameplayAbility> Ability,bool bAllowRemoteActivation) override;
};
