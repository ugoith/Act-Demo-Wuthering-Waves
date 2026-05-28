// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DisplacementComponent.generated.h"

USTRUCT(BlueprintType)
struct FDisplacementConfigParams
{
	GENERATED_BODY()
	FDisplacementConfigParams();
	FDisplacementConfigParams(FVector InVector,FRotator InRotator,FVector2D InInterpSpeed,bool bInUseLocation = true,
		bool bInUseRotation = false):
	DisplacementOffset(InVector),DisplacementRotationOffset(InRotator),bUseLocation(bInUseLocation),bUseRotation(bInUseRotation),
	InterpSpeed(InInterpSpeed){}
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector DisplacementOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FRotator DisplacementRotationOffset = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bUseLocation = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bUseRotation = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2D InterpSpeed = FVector2D::ZeroVector;
	
	
	
};


UCLASS(ClassGroup=(Custom),Blueprintable,BlueprintType, meta=(BlueprintSpawnableComponent))
class ACT_API UDisplacementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDisplacementComponent();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FDisplacementConfigParams DisplacementConfigParams;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector DisplacementLocation;
	//Deprecate
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FRotator DisplacementRotation;
	
	UFUNCTION(BlueprintCallable)
	void DisplacementStart(FDisplacementConfigParams ConfigParams);

	UFUNCTION(BlueprintCallable)
	void UpdateDisplacement(float DeltaSeconds);
	
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector CalculateDisplacementLocation() const;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
