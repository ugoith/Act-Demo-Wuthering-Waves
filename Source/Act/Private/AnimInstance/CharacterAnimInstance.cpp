// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/CharacterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

void UCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	UpdateMovementStatesC();
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (!OwnerPawn) return;
	if (UCharacterMovementComponent* InCMC = OwnerPawn->GetComponentByClass<UCharacterMovementComponent>())
	CMC = InCMC;
}

void UCharacterAnimInstance::UpdateMovementStatesC()
{
	UCharacterMovementComponent* InCMC = CMC.Get();
	if (!InCMC) return;
	//世界坐标系
	WorldVelocity = InCMC->Velocity;
	
	//局部坐标系
	if (!OwnerPawn)
	return;
	Rotation = OwnerPawn->GetActorRotation();
	LocalVelocity = Rotation.UnrotateVector(InCMC->Velocity); 
	
}

UCharacterMovementComponent* UCharacterAnimInstance::GetCMC() const
{
	return CMC.Get();
}
