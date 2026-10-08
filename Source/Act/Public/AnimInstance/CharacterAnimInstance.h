// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActAnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class ACT_API UCharacterAnimInstance : public UActAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
protected:
	virtual void NativeInitializeAnimation() override;

	void UpdateMovementStatesC(float DeltaSeconds);
	void UpdateRotationAndDirectionC(float DeltaSeconds);
	void UpdateAirStatesC(float DeltaSeconds);

	UCharacterMovementComponent* GetCMC() const;

private:
	
	TWeakObjectPtr<UCharacterMovementComponent> CMC;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta = (AllowPrivateAccess = true))
	FVector WorldVelocity;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta = (AllowPrivateAccess = true))
	FVector LastWorldVelocity = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta = (AllowPrivateAccess = true))
	FVector WorldVelocityXY;
	
	UPROPERTY(BlueprintReadOnly,Category = "Movement States",meta =(AllowPrivateAccess = true))
	FVector LocalVelocity;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	float Speed = 0.f;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	float SpeedXY = 0.f;
	
	UPROPERTY(BlueprintReadOnly,Category="RotationAndDirection",meta =(AllowPrivateAccess = true))
	FRotator Rotation;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	FVector WorldAcceleration;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	FVector WorldAccelerationXY;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	FVector LocalAcceleration;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	float AccelerationScalarValue = 0.f;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	float AccelerationScalarValueXY = 0.f;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	FVector AccelerationBasedVelocity = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	FVector CurrentLocation = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	FVector LastLocation = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	bool bFirstUpdate = true;
	
	//基于位移变化的速度，用于AI
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	FVector VelocityBasedLocation;
	
	//速度XY的方向
	UPROPERTY(BlueprintReadOnly,Category="RotationAndDirection",meta =(AllowPrivateAccess = true))
	float VelocityXYDirection = 0.f;
	
	//加速度XY的方向
	UPROPERTY(BlueprintReadOnly,Category="RotationAndDirection",meta =(AllowPrivateAccess = true))
	float AccelerationXYDirection = 0.f;
	
	UPROPERTY(BlueprintReadOnly,Category="Air States",meta =(AllowPrivateAccess = true))
	bool bInAir = false;
	
	UPROPERTY(BlueprintReadOnly,Category="Air States",meta =(AllowPrivateAccess = true))
	bool bIsMovingOnGround = true;
	
	UPROPERTY(BlueprintReadOnly,Category="Air States",meta =(AllowPrivateAccess = true))
	bool bIsRising = false;
};
