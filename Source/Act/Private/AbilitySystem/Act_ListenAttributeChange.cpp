// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Act_ListenAttributeChange.h"

#include "AbilitySystemComponent.h"

UAct_ListenAttributeChange* UAct_ListenAttributeChange::ListenAttributeChange(UAbilitySystemComponent* ASC,
                                                                              FGameplayAttribute Attribute)
{
	UAct_ListenAttributeChange* ListenAttributeChange = NewObject<UAct_ListenAttributeChange>();
	ListenAttributeChange->AbilitySystemComponent = ASC;
	ListenAttributeChange->AttributeToListenFor = Attribute;
	
	if (!ASC)
	{
		ListenAttributeChange->RemoveFromRoot();
		return nullptr;
	}
	ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(ListenAttributeChange,&UAct_ListenAttributeChange::BindAttributeChangeCallback);
	return ListenAttributeChange;
}

void UAct_ListenAttributeChange::EndTask()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeToListenFor).RemoveAll(this);
	}
	
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UAct_ListenAttributeChange::BindAttributeChangeCallback( const FOnAttributeChangeData& ChangedData)
{
	OnAttributeChangedSignature.Broadcast(ChangedData.Attribute,ChangedData.NewValue,ChangedData.OldValue);
}
