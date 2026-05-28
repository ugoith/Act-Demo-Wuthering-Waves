// Fill out your copyright notice in the Description page of Project Settings.
#include "Act/Public/Game/ActPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ActGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/Ability/ActGameplayAbilityBase.h"
#include "Character/ActCharacterBase.h"
#include "Component/InputInfoManager.h"
#include "Data/DataAsset/ControllerInputConfig.h"
#include "Evaluation/IMovieSceneEvaluationHook.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

void AActPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//服务器直接返回
	if (!IsLocalController()) return;
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(ControllerInputConfig->InputMappingContext,0);
			if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
			{
					// 使用ControllerInputConfig绑定Move、Jump和Look
				EnhancedInputComp->BindAction(ControllerInputConfig->InputActions.FindChecked(InputActions::Move::InputActions_Move),
					ETriggerEvent::Triggered, this, &AActPlayerController::AddMovementInput);
				EnhancedInputComp->BindAction(ControllerInputConfig->InputActions.FindChecked(InputActions::Jump::InputActions_Jump),
					ETriggerEvent::Started, this, &AActPlayerController::AddJumpInput);
				EnhancedInputComp->BindAction(ControllerInputConfig->InputActions.FindChecked(InputActions::Jump::InputActions_Jump),
					ETriggerEvent::Completed, this, &AActPlayerController::AddJumpInput_Complete);
				EnhancedInputComp->BindAction(ControllerInputConfig->InputActions.FindChecked(InputActions::Look::InputActions_Look),
					ETriggerEvent::Triggered, this, &AActPlayerController::AddCameraInput);
				BindInputActionEvents(EnhancedInputComp);
			}
		}
	}
	
	
}

void AActPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ControlledCharacter = Cast<ACharacter>(InPawn);
	ControlledCharacterMovementComponent = ControlledCharacter ? ControlledCharacter->GetCharacterMovement() : nullptr;
}

void AActPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
	ControlledCharacter = Cast<ACharacter>(P);
	ControlledCharacterMovementComponent = ControlledCharacter ? ControlledCharacter->GetCharacterMovement() : nullptr;
}

void AActPlayerController::AddMovementInput(const FInputActionValue& Value)
{
	const FVector MoveAxisVector = Value.Get<FVector>();
	const FRotator ControlRotator = GetControlRotation();
	const FRotator YawRotator(0.f,ControlRotator.Yaw,0.f);
	const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	if (AActCharacterBase* ActCharacter= GetPawn<AActCharacterBase>())
	{
		ActCharacter->AddMovementInput(ForwardVector,MoveAxisVector.Y);
		ActCharacter->AddMovementInput(RightVector,MoveAxisVector.X);

		IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(ActCharacter);
		if (!ASI) return;

		UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
		if (!ASC) return;

		USkeletalMeshComponent* Mesh = ActCharacter->GetMesh();
		if (!Mesh || !Mesh->GetAnimInstance()) return;

		// 检查是否需要停止蒙太奇：有CanMove标签 或 蒙太奇已结束
		FGameplayTagContainer OwnedGameplayTags;
		ASC->GetOwnedGameplayTags(OwnedGameplayTags);
		const bool bHasCanMoveTag = OwnedGameplayTags.HasTagExact(BattleActions::Interrupt::BattleActions_Interrupt_CanMove);
		const bool bMontageEnded = !Mesh->GetAnimInstance()->GetCurrentActiveMontage();

		if (bHasCanMoveTag || bMontageEnded)
		{
			UActGameplayAbilityBase* GameplayAbility = Cast<UActGameplayAbilityBase>(ASC->GetAnimatingAbility());
			if (GameplayAbility)
			{
				ASC->CurrentMontageStop();
			}
		}
	}
}

void AActPlayerController::AddJumpInput(const FInputActionValue& Value)
{
	if (ACharacter* ActCharacter = GetCharacter())
	{
		IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(ActCharacter);
		if (!ASI) return;

		UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
		if (!ASC) return;
		
		if (!ActCharacter->GetCharacterMovement()->IsFalling())
		{
			ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(InputActions::Jump::InputActions_Jump))? ActCharacter->Jump(): ActCharacter->StopJumping();
		}
	}
}

void AActPlayerController::AddCameraInput(const FInputActionValue& Value)
{
	FVector CameraAxisVector = Value.Get<FVector>();
	if (APawn* ActPawn = GetPawn<APawn>())
	{
		ActPawn->AddControllerYawInput(CameraAxisVector.X);
		ActPawn->AddControllerPitchInput(CameraAxisVector.Y);
	}
}

void AActPlayerController::AddJumpInput_Complete(const FInputActionValue& Value)
{
	if (ACharacter* ActCharacter = GetCharacter())
	{
		ActCharacter->StopJumping();
	}
}

void AActPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AActPlayerController::BindInputActionEvents(UEnhancedInputComponent* EnhancedInputComponent)
{
	for (const TTuple<FGameplayTag, FInputActionTriggerConfig>& InputActionConfig : ControllerInputConfig->InputActionConfig)
	{
		for (const FTriggerEventNeedTriggerAbilities& TriggerEventConfig : InputActionConfig.Value.TriggerConfig)
		{
			EnhancedInputComponent->BindAction(InputActionConfig.Value.InputAction, TriggerEventConfig.TriggerEvent, this,
				&AActPlayerController::HandleTriggeredInput,TriggerEventConfig);
		}
	}
}

void AActPlayerController::TryActivateAbilities(const FGameplayTagContainer& TagsToActivate,const TArray<TSubclassOf<UGameplayAbility>>& GameplayAbilities)
{
	if (!GetPawn()) return;
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()))
	{
		if (!TagsToActivate.IsEmpty())
		{
			ASC->TryActivateAbilitiesByTag(TagsToActivate);
		}
		for (const TSubclassOf<UGameplayAbility>& GameplayAbility : GameplayAbilities)
		{
			ASC->TryActivateAbilityByClass(GameplayAbility);
		}
	}
}

void AActPlayerController::HandleTriggeredInput(const FInputActionValue& Value,const FTriggerEventNeedTriggerAbilities TriggerEventConfig)
{
	
	UInputInfoManager* InputMgr = Cast<UInputInfoManager>(GetComponentByClass(UInputInfoManager::StaticClass()));
	if (InputMgr)
	{
		if (TriggerEventConfig.TriggerEvent == ETriggerEvent::Canceled || TriggerEventConfig.TriggerEvent == ETriggerEvent::Completed)
		{
			InputMgr->ResetAllParams();
		}
		else
		{
			InputMgr->UpdatePreInput(TriggerEventConfig.PreInputTagToSend);
		}
	}
	
	if (TriggerEventConfig.AbilitiesToTrigger.IsEmpty() && TriggerEventConfig.AbilitiesTags.IsEmpty()) return;
	if (!GetPawn()) return;
	TryActivateAbilities(TriggerEventConfig.AbilitiesTags, TriggerEventConfig.AbilitiesToTrigger);
}
