// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ActHUD.h"

#include "Blueprint/UserWidget.h"
#include "Controller/WidgetController.h"
#include "UI/Act_UserWidget.h"


void AActHUD::BeginPlay()
{
	Super::BeginPlay();
	
}

void AActHUD::InitWidget(AActPlayerController* PC, AActPlayerState* PS, APawn* Pawn, UAbilitySystemComponent* ASC,
	UActAttributeSet* AS)
{
	checkf(HUDWidgetClass,TEXT("Please Fill Out HUDWidgetClass"));
	checkf(WidgetControllerClass,TEXT("Please Fill Out WidgetControllerClass"));
    
	PlayerMainUI = CreateWidget<UAct_UserWidget>(GetWorld(),HUDWidgetClass);
    
	WidgetController = NewObject<UWidgetController>(this,WidgetControllerClass);
	WidgetController->SetWidgetControllerParams(PC,Pawn,PS,ASC,AS);
    
	//  先给 UI
	PlayerMainUI->SetWidgetController(WidgetController);

	//  再绑定 ASC,广播属性变化事件
	WidgetController->BindCallbacks();

	//  最后推初始值（关键！！！）
	WidgetController->BroadcastInitialValues();
	PlayerMainUI->AddToViewport();
}
