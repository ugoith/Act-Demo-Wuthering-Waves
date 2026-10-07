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
	
	virtual void NativeInitializeAnimation() override;

	void UpdateMovementStatesC();
	
	UCharacterMovementComponent* GetCMC() const;

private:
	
	TWeakObjectPtr<UCharacterMovementComponent> CMC;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta = (AllowPrivateAccess = true))
	FVector WorldVelocity;
	
	UPROPERTY(BlueprintReadOnly,Category = "Movement States",meta =(AllowPrivateAccess = true))
	FVector LocalVelocity;
	
	UPROPERTY(BlueprintReadOnly,Category="Movement States",meta =(AllowPrivateAccess = true))
	FRotator Rotation;
};
