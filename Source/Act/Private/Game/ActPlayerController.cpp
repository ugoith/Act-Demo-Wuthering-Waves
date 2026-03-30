// Fill out your copyright notice in the Description page of Project Settings.
#include "Act/Public/Game/ActPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"

void AActPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//服务器直接返回
	if (!IsLocalController()) return;
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(InputMappingContext,0);
			if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
			{
				EnhancedInputComp->BindAction(IA_Move,ETriggerEvent::Triggered,this,&AActPlayerController::AddMovementInput);
				EnhancedInputComp->BindAction(IA_Jump,ETriggerEvent::Started,this,&AActPlayerController::AddJumpInput);
				EnhancedInputComp->BindAction(IA_Camera,ETriggerEvent::Triggered,this,&AActPlayerController::AddCameraInput);
				EnhancedInputComp->BindAction(IA_Jump,ETriggerEvent::Completed,this,&AActPlayerController::AddJumpInput);
			}
		}
	}
	
	
}

void AActPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}

void AActPlayerController::AddMovementInput(const FInputActionValue& Value)
{
	const FVector MoveAxisVector = Value.Get<FVector>();
	const FRotator ControlRotator = GetControlRotation();
	const FRotator YawRotator(0.f,ControlRotator.Yaw,0.f);
	const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
	if (APawn* ActPawn= GetPawn<APawn>())
	{
		ActPawn->AddMovementInput(ForwardVector,MoveAxisVector.Y);
		ActPawn->AddMovementInput(RightVector,MoveAxisVector.X);
	}
}

void AActPlayerController::AddJumpInput(const FInputActionValue& Value)
{
	if (ACharacter* ActCharacter = GetCharacter())
	{
		ActCharacter->Jump();
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
