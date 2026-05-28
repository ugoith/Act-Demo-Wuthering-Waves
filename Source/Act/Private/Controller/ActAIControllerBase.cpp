// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ActAIControllerBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Perception/AIPerceptionComponent.h"


// Sets default values
AActAIControllerBase::AActAIControllerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this,&AActAIControllerBase::HandlePerceptionUpdated);
}

// Called when the game starts or when spawned
void AActAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AActAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunBehaviorTree(BehaviorTree);
	AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InPawn);
	
}

// Called every frame
void AActAIControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActAIControllerBase::HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	
}

bool AActAIControllerBase::ActivateControlledAbilities(TSubclassOf<UGameplayAbility> Ability,
	bool bAllowRemoteActivation)
{
	return AbilitySystemComponent->TryActivateAbilityByClass(Ability,bAllowRemoteActivation);
}
