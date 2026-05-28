// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AppearanceController.h"

#include "AbilitySystem/Act_ListenGenericGameplayTagChange.h"
#include "BlueprintFunctionLibrary/ActBlueprintFunctionLibrary.h"
#include "Interface/AppearanceInterface.h"


// Sets default values for this component's properties
UAppearanceController::UAppearanceController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAppearanceController::BeginPlay()
{
	Super::BeginPlay();
	// ...
	UAbilitySystemComponent* ASC = UActBlueprintFunctionLibrary::GetOwnerAbililtySystemComponent(this);
	if (!ASC) return;
	RegisterGenericGameplayTagChangedDelegateHandle = ASC->RegisterGenericGameplayTagEvent().AddUObject(this,&ThisClass::ReceiveOnGameplayTagChanged);
	
}

void UAppearanceController::Deactivate()
{
	Super::Deactivate();
	UAbilitySystemComponent* ASC = UActBlueprintFunctionLibrary::GetOwnerAbililtySystemComponent(this);
	if (!ASC || !RegisterGenericGameplayTagChangedDelegateHandle.IsValid()) return;
	ASC->RegisterGenericGameplayTagEvent().Remove(RegisterGenericGameplayTagChangedDelegateHandle);
}


// Called every frame
void UAppearanceController::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAppearanceController::GetOwnerSubMeshes()
{
	IAppearanceInterface* AppearanceInterface = Cast<IAppearanceInterface>(GetOwner());
	if (!AppearanceInterface) return;
	AppearanceInterface->Execute_GetSubMeshes(GetOwner(),OwnerSubMeshesInfo);
}

void UAppearanceController::ReceiveOnGameplayTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	UAbilitySystemComponent* ASC = UActBlueprintFunctionLibrary::GetOwnerAbililtySystemComponent(this);
	if (!ASC) return;
	for (const FAppearanceChangeConfigWhenTagChange& TagChangeAppearanceConfig : AppearanceChangeConfigWhenTagChanges)
	{
		if (TagChangeAppearanceConfig.Tag.MatchesTagExact(Tag))
		{
			if ((ASC->GetTagCount(Tag) > 0 && TagChangeAppearanceConfig.bOnAdded) || (ASC->GetTagCount(Tag) == 0 && !TagChangeAppearanceConfig.bOnAdded))
			{
				for (const TPair<FGameplayTag,bool>& MeshVisibilityConfig : TagChangeAppearanceConfig.MeshesVisibilityConfig)
				{
					USceneComponent* MeshComponent = OwnerSubMeshesInfo.FindChecked(MeshVisibilityConfig.Key);
					MeshComponent->SetVisibility(MeshVisibilityConfig.Value);
				}
			}
		}
	}
	
}

