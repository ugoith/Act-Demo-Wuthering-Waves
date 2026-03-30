// Fill out your copyright notice in the Description page of Project Settings.


#include "Act/Public/Character/EnemyCharacterBase.h"
#include "AbilitySystem/ActAbilitySystemComponent.h"
#include "AbilitySystem/ActAttributeSet.h"


// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UActAbilitySystemComponent>(FName("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UActAttributeSet>(FName("AttributeSet"));
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* AEnemyCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

