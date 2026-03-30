// Fill out your copyright notice in the Description page of Project Settings.


#include "Act/Public/Character/ActCharacterBase.h"

#include "Components/CapsuleComponent.h"


// Sets default values
AActCharacterBase::AActCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
}

// Called when the game starts or when spawned
void AActCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	for (const TObjectPtr<USceneComponent>& ChildComponent : GetMesh()->GetAttachChildren())
	{
		Cast<USkeletalMeshComponent>(ChildComponent)->SetLeaderPoseComponent(GetMesh());
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

