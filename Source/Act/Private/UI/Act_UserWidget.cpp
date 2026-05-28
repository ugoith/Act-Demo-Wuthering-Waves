// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Act_UserWidget.h"

#include "AbilitySystemComponent.h"
#include "UnrealWidgetFwd.h"
#include "Controller/WidgetController.h"


void UAct_UserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

/*void UAct_UserWidget::BindAttributeChanges()
{
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwningPlayerPawn());
	if (!ASI) return;
	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC) return;
	if (!BindAttributes.Num()) return;
	for (const FGameplayAttribute& Attribute : BindAttributes )
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda
		([this,Attribute]( const FOnAttributeChangeData& AttributeChangeData)
		{
			BP_OnAttributeChange(Attribute,AttributeChangeData.NewValue,AttributeChangeData.OldValue);
		});
	}
}*/

void UAct_UserWidget::SetWidgetController(UWidgetController* NewWidgetController)
{
	WidgetController = NewWidgetController;

	if (!WidgetController) return;

	//  绑定 Controller 事件（不是 ASC！）
	WidgetController->OnAttributeChanged.AddDynamic(
		this,
		&UAct_UserWidget::HandleAttributeChanged
	);

	// 这个你保留给蓝图
	WidgetControllerHasSet();
}
void UAct_UserWidget::HandleAttributeChanged(FGameplayAttribute Attribute, float NewValue, float OldValue)
{
	BP_OnAttributeChange(Attribute, NewValue, OldValue);
}
