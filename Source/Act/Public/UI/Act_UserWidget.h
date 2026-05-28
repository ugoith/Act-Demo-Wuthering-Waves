// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "Act_UserWidget.generated.h"

class UWidgetController;
class UActAttributeSet;

/**
 * 
 */
UCLASS()
class ACT_API UAct_UserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetController> WidgetController;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FGameplayAttribute> BindAttributes;
	virtual void NativeOnInitialized() override;
	/*UFUNCTION(BlueprintCallable)
	void BindAttributeChanges();*/
	UFUNCTION(BlueprintImplementableEvent,meta = (DisplayName = "OnAttributeChange"))
	void BP_OnAttributeChange(FGameplayAttribute AttributeHasChanged,float NewValue ,float OldValue);

	void SetWidgetController(UWidgetController* NewWidgetController);
	UFUNCTION()
	void HandleAttributeChanged(FGameplayAttribute Attribute, float NewValue, float OldValue);

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerHasSet();
	
	
	
};
