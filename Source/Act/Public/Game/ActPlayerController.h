// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "ActPlayerController.generated.h"
struct FTriggerEventNeedTriggerAbilities;
struct FGameplayTagContainer;
class UGameplayAbility;
class UCharacterMovementComponent;
struct FInputActionValue;
class UInputMappingContext;
class UControllerInputConfig;
/**
 * 
 */


UCLASS()
class ACT_API AActPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Input")
	TObjectPtr<UControllerInputConfig> ControllerInputConfig;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "ControlledPawnInfo")
	TObjectPtr<UCharacterMovementComponent> ControlledCharacterMovementComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<ACharacter> ControlledCharacter;

public:
	virtual void AcknowledgePossession(class APawn* P) override;
	void AddMovementInput(const FInputActionValue& Value);
	void AddJumpInput(const FInputActionValue& Value);
	void AddCameraInput(const FInputActionValue& Value);
	void AddJumpInput_Complete(const FInputActionValue& Value);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void BindInputActionEvents(UEnhancedInputComponent* EnhancedInputComponent);
	void TryActivateAbilities(const FGameplayTagContainer& TagsToActivate,const TArray<TSubclassOf<UGameplayAbility>>& GameplayAbilities);
	void HandleTriggeredInput(const FInputActionValue& Value,const FTriggerEventNeedTriggerAbilities TriggerEventConfig);
	

protected:
	
private:
	
	
};
