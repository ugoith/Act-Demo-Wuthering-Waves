// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/DisplacementComponent.h"


FDisplacementConfigParams::FDisplacementConfigParams()
{
	
}

// Sets default values for this component's properties
UDisplacementComponent::UDisplacementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UDisplacementComponent::DisplacementStart(FDisplacementConfigParams ConfigParams)
{
	DisplacementConfigParams = ConfigParams;
	DisplacementLocation = CalculateDisplacementLocation();
	DisplacementRotation = GetOwner()->GetActorRotation() + DisplacementConfigParams.DisplacementRotationOffset;
}

void UDisplacementComponent::UpdateDisplacement(float DeltaSeconds)
{
	if (!GetOwner()) return;
	if (DisplacementConfigParams.bUseLocation)
	{
		FVector InterpLocation = FMath::VInterpTo(GetOwner()->GetActorLocation(), 
		DisplacementLocation,DeltaSeconds,
		DisplacementConfigParams.InterpSpeed.X);	
		GetOwner()->SetActorLocation(InterpLocation,true);
	}
	if (DisplacementConfigParams.bUseRotation)
	{
		FRotator InterRotator = FMath::RInterpTo(GetOwner()->GetActorRotation(),DisplacementRotation,
			DeltaSeconds,DisplacementConfigParams.InterpSpeed.Y);
		
		GetOwner()->SetActorRotation(InterRotator);
	}
}

FVector UDisplacementComponent::CalculateDisplacementLocation() const
{
	AActor* Owner = GetOwner();
	if (!GetOwner()) return FVector(0, 0, 0);
	const FVector ForwardVector = Owner->GetActorForwardVector();
	const FVector ForwardOffset = DisplacementConfigParams.DisplacementOffset.X * ForwardVector;
	
	const FVector RightVector = Owner->GetActorRightVector();
	const FVector RightOffset = DisplacementConfigParams.DisplacementOffset.Y * RightVector;
	
	const FVector UpVector = Owner->GetActorUpVector();
	const FVector UpOffset = DisplacementConfigParams.DisplacementOffset.Z * UpVector;
	
	return Owner->GetActorLocation() + ForwardOffset + RightOffset + UpOffset;
}

// Called when the game starts
void UDisplacementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDisplacementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

