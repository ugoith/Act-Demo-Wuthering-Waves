// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ActHUD.generated.h"

class UActAttributeSet;
class UAbilitySystemComponent;
class UWidgetController;
class AActPlayerState;
class AActPlayerController;
class UAct_UserWidget;
/**
 * 
 */
UCLASS()
class ACT_API AActHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UAct_UserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UWidgetController> WidgetControllerClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetController> WidgetController;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UAct_UserWidget> PlayerMainUI;
	
	virtual void BeginPlay() override;
	
	void InitWidget(AActPlayerController* PC,AActPlayerState* PS,APawn* Pawn,UAbilitySystemComponent* ASC,UActAttributeSet* AS);
};
