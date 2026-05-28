// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/CheckGameplayTagsOnActorASC.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


UCheckGameplayTagsOnActorASC::UCheckGameplayTagsOnActorASC(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//节点名称
	NodeName = "CheckGameplayTagOnActorASC";
	//NotifyObserver = EBTBlackboardRestart::ResultChange;
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
	
	//过滤掉一些黑板键，只要Object类型，同时需要这个函数InitializeFromAsset
	ActorToCheck.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UCheckGameplayTagsOnActorASC, ActorToCheck),AActor::StaticClass());

	//初始化选择的键值类型
	ActorToCheck.SelectedKeyName = FBlackboard::KeySelf;
	
	// For now, don't allow users to select any "Abort Observers", because it's currently not supported.
	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
	
	FlowAbortMode = EBTFlowAbortMode::Both;	
}

bool UCheckGameplayTagsOnActorASC::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return false;

	// 1. 获取对象：不要使用 <UBlackboardKeyType_Object> 这种模板写法，使用更安全的辅助函数
	UObject* TargetObject = BBComp->GetValueAsObject(ActorToCheck.SelectedKeyName);
	if (!TargetObject)
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("CheckGameplayTags: BB Key '%s' is null!"), *ActorToCheck.SelectedKeyName.ToString());
		return false;
	}

	// 2. 接口转换
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(TargetObject);
	if (!ASI)
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("CheckGameplayTags: Object '%s' (%s) does not implement IAbilitySystemInterface!"), 
			*TargetObject->GetName(), *TargetObject->GetClass()->GetName());
		return false;
	}

	// 3. 获取 ASC
	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("CheckGameplayTags: ASC is null on object '%s'!"), *TargetObject->GetName());
		return false;
	}
	
	FGameplayTagContainer OwnedTags;
	ASC->GetOwnedGameplayTags(OwnedTags);
	
	switch (TagsToMatch)
	{
	case EGameplayContainerMatchType::Any:
		{
			return OwnedTags.HasAnyExact(GameplayTags);
		}
	case EGameplayContainerMatchType::All:
		{
			return OwnedTags.HasAllExact(GameplayTags);
		}
	default:
		{
			UE_LOG(LogBehaviorTree, Warning, TEXT("Invalid value for TagsToMatch (EGameplayContainerMatchType) %d.  Should only be Any or All."), static_cast<int32>(TagsToMatch));
			return false;
		}
	}
	
}

void UCheckGameplayTagsOnActorASC::InitializeFromAsset(UBehaviorTree& Asset)
{
	//必须加上这个用来设置行为树的，不然行为树就是空值，GetBlackboardAsset就是空值
	Super::InitializeFromAsset(Asset);
	//获取行为树的黑板
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		//查找选择的键值
		ActorToCheck.ResolveSelectedKey(*BBAsset);
	}
	
}

UAbilitySystemComponent* UCheckGameplayTagsOnActorASC::GetActorToCheckAbilitySystemComp(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		UObject* TargetObject = BlackboardComp->GetValueAsObject(ActorToCheck.SelectedKeyName);
		IAbilitySystemInterface* IAbilitySystem = Cast<IAbilitySystemInterface>(TargetObject);
		if (!IAbilitySystem) return nullptr;
		UAbilitySystemComponent* ASC = IAbilitySystem->GetAbilitySystemComponent();
		return ASC;
	}
	return nullptr;
}

void UCheckGameplayTagsOnActorASC::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(ActorToCheck.SelectedKeyName));
	if (!ASI) return;
	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	OnGameplayTagCountChanged = ASC->RegisterGenericGameplayTagEvent().AddLambda([this,&OwnerComp]
		(const FGameplayTag& Tag, int32 Count)
	{
		//再次执行条件检查
		OwnerComp.RequestExecution(this);
	});;
	
}

void UCheckGameplayTagsOnActorASC::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UAbilitySystemComponent* ASC = GetActorToCheckAbilitySystemComp(OwnerComp);
	if (!ASC) return;
	if (OnGameplayTagCountChanged.IsValid())
	{
		ASC->RegisterGenericGameplayTagEvent().Remove(OnGameplayTagCountChanged);
	}
	OnGameplayTagCountChanged.Reset();
}
