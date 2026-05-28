// Fill out your copyright notice in the Description page of Project Settings.


#include "Act/Public/Character/ActCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/ActAttributeSet.h"
#include "UI/Act_UserWidget.h"
#include "GameplayTagsManager.h"
#include "GameplayTagContainer.h"

// Sets default values
AActCharacterBase::AActCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
	MotionWarpingComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AActCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	for (const TObjectPtr<USceneComponent>& ChildComponent : GetMesh()->GetAttachChildren())
	{
		USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(ChildComponent);
		if (!SkeletalMeshComponent) continue;
		if (SkeletalMeshComponent->GetAnimInstance()) continue;
		SkeletalMeshComponent->SetLeaderPoseComponent(GetMesh(),true,true);
	}
	
}

void AActCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!HasAuthority()) return;
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC) return;
	for (const TSubclassOf<UGameplayAbility> Ability : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec(Ability,1.0,INDEX_NONE,ASC->GetAvatarActor());
		ASC->GiveAbility(AbilitySpec);
	}
}

// Called every frame
void AActCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AActCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* AActCharacterBase::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AActCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,MotionWarpingComponent);
	DOREPLIFETIME(ThisClass,Abilities);
	DOREPLIFETIME(ThisClass,TeamID);
}

void AActCharacterBase::InitializeEffectsToSelf()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const TSubclassOf<UGameplayEffect>& GameplayEffect : InitializeEffects)
		{
			FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect,1.0,ContextHandle);
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
	
}

void AActCharacterBase::InitAbiltityActorInfo()
{
	
}

FGenericTeamId AActCharacterBase::GetGenericTeamId() const
{
	return TeamID;
}

ETeamAttitude::Type AActCharacterBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	
	const IGenericTeamAgentInterface* OtherTeam = Cast<IGenericTeamAgentInterface>(&Other);
	if (!OtherTeam) return ETeamAttitude::Type::Neutral;
	
	if (OtherTeam->GetGenericTeamId() == GetGenericTeamId())
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		return ETeamAttitude::Hostile;
	}
}

