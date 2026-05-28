// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/WidgetController.h"

#include "AbilitySystemComponent.h"
#include "Game/ActPlayerController.h"
#include "Game/ActPlayerState.h"

UWidgetController::UWidgetController()
{
	
}

void UWidgetController::SetWidgetControllerParams(AActPlayerController* InPC, APawn* InPawn, AActPlayerState* InPS,
	UAbilitySystemComponent* InASC,UActAttributeSet* InAS)
{
	PlayerController = InPC;
	PlayerState = InPS;
	PlayerPawn = InPawn;
	AbilitySystemComponent = InASC;
	AttributeSet = InAS;
}
void UWidgetController::BindCallbacks()
{
	if (!AbilitySystemComponent) return;

	for (const FGameplayAttribute& Attr : BindAttributes) // 你可以复用
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attr)
		.AddLambda([this, Attr](const FOnAttributeChangeData& Data)
		{
			OnAttributeChanged.Broadcast(Attr, Data.NewValue, Data.OldValue);
		});
	}
}

void UWidgetController::BroadcastInitialValues()
{
	if (!AbilitySystemComponent) return;

	for (const FGameplayAttribute& Attr : BindAttributes)
	{
		float Value = AbilitySystemComponent->GetNumericAttribute(Attr);

		OnAttributeChanged.Broadcast(Attr, Value, Value);
	}
}