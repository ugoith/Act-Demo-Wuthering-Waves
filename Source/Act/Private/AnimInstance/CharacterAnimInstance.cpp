// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/CharacterAnimInstance.h"

#include "Elements/Framework/TypedElementQueryBuilder.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	UpdateMovementStatesC(DeltaSeconds);
	UpdateRotationAndDirectionC(DeltaSeconds);
	UpdateAirStatesC(DeltaSeconds);
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (!OwnerPawn) return;
	if (UCharacterMovementComponent* InCMC = OwnerPawn->GetComponentByClass<UCharacterMovementComponent>())
	CMC = InCMC;
}

void UCharacterAnimInstance::UpdateMovementStatesC(float DeltaSeconds)
{
	UCharacterMovementComponent* InCMC = CMC.Get();
	if (!InCMC) return;
	//世界坐标系
	if (bFirstUpdate)
		WorldVelocity = InCMC->Velocity;
	LastWorldVelocity = WorldVelocity;
	WorldVelocity = InCMC->Velocity;
	WorldVelocityXY = FVector(WorldVelocity.X, WorldVelocity.Y, 0);
	WorldAcceleration = InCMC->GetCurrentAcceleration();
	WorldAccelerationXY = FVector(WorldAcceleration.X, WorldAcceleration.Y, 0);
	AccelerationBasedVelocity = (WorldVelocity - LastWorldVelocity)/DeltaSeconds;
	Speed = WorldVelocity.Size();
	SpeedXY = WorldVelocityXY.Size();
	AccelerationScalarValue = WorldAcceleration.Size();
	AccelerationScalarValueXY = WorldAccelerationXY.Size();
	
	//局部坐标系
	if (!OwnerPawn)
	return;
	Rotation = OwnerPawn->GetActorRotation();
	LocalVelocity = Rotation.UnrotateVector(InCMC->Velocity); 
	LocalAcceleration = Rotation.UnrotateVector(WorldAcceleration);
	
	//基于位移的变化的速度计算
	if (bFirstUpdate)
		CurrentLocation = OwnerPawn->GetActorLocation();
	LastLocation = CurrentLocation;
	CurrentLocation = OwnerPawn->GetActorLocation();
	VelocityBasedLocation = (CurrentLocation - LastLocation)/DeltaSeconds;
	
	bFirstUpdate = false;
}

void UCharacterAnimInstance::UpdateRotationAndDirectionC(float DeltaSeconds)
{
	VelocityXYDirection = CalculateDirection(WorldVelocityXY,Rotation);
	AccelerationXYDirection = CalculateDirection(WorldAccelerationXY,Rotation);
}

void UCharacterAnimInstance::UpdateAirStatesC(float DeltaSeconds)
{
	UCharacterMovementComponent* InCMC = CMC.Get();
	if (!InCMC)
		return;
	bInAir = CMC->IsFalling();
	bIsMovingOnGround = CMC->IsMovingOnGround();
	bIsRising = WorldVelocity.Z > 0;
}

UCharacterMovementComponent* UCharacterAnimInstance::GetCMC() const
{
	return CMC.Get();
}
