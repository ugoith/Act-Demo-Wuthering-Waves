// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibrary/ActBlueprintFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Math/MathFwd.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "BlueprintGameplayTagLibrary.h"
#include "AbilitySystem/Ability/ActGameplayAbilityBase.h"
#include "Component/HitAndDamageComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"


UActorComponent* UActBlueprintFunctionLibrary::GetOwnerComponentByClass(UActorComponent* Target,
                                                                        TSubclassOf<UActorComponent> ComponentClass)
{
	if(!Target || !ComponentClass) return nullptr;
	if(!Target->GetOwner()) return nullptr;
	return Target->GetOwner()->GetComponentByClass(ComponentClass);
}

UAbilitySystemComponent* UActBlueprintFunctionLibrary::GetOwnerAbililtySystemComponent(UActorComponent* Target)
{
	AActor* Actor = Target->GetOwner();
	if(!Actor) return nullptr;
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Actor);
	UAbilitySystemComponent* ASC = ASI ? ASI->GetAbilitySystemComponent() : nullptr;
	return ASC;
}

ACharacter* UActBlueprintFunctionLibrary::GetOwnerCharacter(UActorComponent* Target)
{
	return Cast<ACharacter>(Target->GetOwner());
}

void UActBlueprintFunctionLibrary::GetChildrenAnimInstancesByASC(UAbilitySystemComponent* ASC,
                                                                 TArray<UAnimInstance*>& AnimInstances)
{
	USkeletalMeshComponent* Mesh = ASC->AbilityActorInfo->SkeletalMeshComponent.Get();
	if(!Mesh) return;
	TArray<USceneComponent*> ChildrenComponents;
	Mesh->GetChildrenComponents(true,ChildrenComponents);
	for (USceneComponent* Child : ChildrenComponents)
	{
		USkeletalMeshComponent* SubMesh = Cast<USkeletalMeshComponent>(Child);
		if(!SubMesh) continue;
		AnimInstances.Add(SubMesh->GetAnimInstance());
	}
}

bool UActBlueprintFunctionLibrary::QueryTargetActorOwnedTags(AActor* Target, FGameplayTagQuery Query)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
	{
		FGameplayTagContainer TargetOwnedTags = FGameplayTagContainer();
		ASC->GetOwnedGameplayTags(TargetOwnedTags);
		return UBlueprintGameplayTagLibrary::IsTagQueryEmpty(Query) || UBlueprintGameplayTagLibrary::DoesContainerMatchTagQuery(TargetOwnedTags, Query);
	}
	return false;
}

bool UActBlueprintFunctionLibrary::QueryTags(FGameplayTagContainer TagsToQuery, FGameplayTagQuery Query)
{
	return UBlueprintGameplayTagLibrary::IsTagQueryEmpty(Query) || UBlueprintGameplayTagLibrary::DoesContainerMatchTagQuery(TagsToQuery, Query);
}

FVector UActBlueprintFunctionLibrary::CalculateActorToTargetActorVector(AActor* From, AActor* To)
{
	const FVector Actor = From->GetActorLocation();
	const FVector ToActor= To->GetActorLocation();
	return ToActor-Actor;
}

void UActBlueprintFunctionLibrary::GetThreeDirection(FRotator InRot, FVector& Forward, FVector& Right, FVector& Up)
{
	FMatrix RotationMatrix = FRotationMatrix(InRot);
	Forward = RotationMatrix.GetUnitAxis(EAxis::X);
	Right = RotationMatrix.GetUnitAxis(EAxis::Y);
	Up = RotationMatrix.GetUnitAxis(EAxis::Z);
}

void UActBlueprintFunctionLibrary::DebugFromShapeType(USkeletalMeshComponent* MeshComp, FHitTestConfig HitTestConfig,float DeltaSeconds)
{
	
	if (!HitTestConfig.bDrawPreview) return;
	if (HitTestConfig.AttachBoneName.IsNone())
	{
		HitTestConfig.AttachBoneName = FName("Root");
	}
	//计算插槽各个方向的单位向量
	const FRotator& Rotation = MeshComp->GetSocketRotation(HitTestConfig.AttachBoneName);
	const FRotationMatrix& SocketRotation = FRotationMatrix(Rotation);
	const FVector& SocketForwardVector = SocketRotation.GetUnitAxis(EAxis::X);
	const FVector& SocketRightVector = SocketRotation.GetUnitAxis(EAxis::Y);
	const FVector& SocketUpVector = SocketRotation.GetUnitAxis(EAxis::Z);
	
	//生成碰撞检测，得到HitResults结果
	for (const FShapeConfigInfo& CollisionConfig : HitTestConfig.CollisionConfigInfo)
	{
		const FVector& StartLocation = MeshComp->GetSocketLocation(HitTestConfig.AttachBoneName) + 
			SocketForwardVector * CollisionConfig.OffsetTransform.GetLocation().X +
			SocketRightVector * CollisionConfig.OffsetTransform.GetLocation().Y +
			SocketUpVector * CollisionConfig.OffsetTransform.GetLocation().Z;
		const FVector& EndLocation = StartLocation;
		
		const FQuat& Rot = Rotation.Quaternion() * CollisionConfig.OffsetTransform.GetRotation();
		FCollisionShape CollisionShape;
		
		switch (CollisionConfig.CollisionShapes)
		{
			case ECollisionShapes::Box:
				{	
					UKismetSystemLibrary::DrawDebugBox(MeshComp,StartLocation,FVector(CollisionConfig.BoxExtent),
						FLinearColor::Red, FRotator(Rot),DeltaSeconds,1.f);
					break;
				}
			case ECollisionShapes::Capsule:
				{
					UKismetSystemLibrary::DrawDebugCapsule(MeshComp,StartLocation,CollisionConfig.CapsuleHalfHeight,
						CollisionConfig.CapsuleRadius,FRotator(Rot),FLinearColor::Red, DeltaSeconds,1.f);
					break;
				}
			case ECollisionShapes::Sphere:
				{
					UKismetSystemLibrary::DrawDebugSphere(MeshComp,StartLocation,CollisionConfig.Radius,18
						,FLinearColor::Red, DeltaSeconds,1.f);
					break;
				}
		}
	}
	
}

bool UActBlueprintFunctionLibrary::IsSatisfyAttributeInspector(const UAbilitySystemComponent* AbilitySystemComponent,
	const FAttributeInspector& AttributeInspector)
{
	const float AttributeValue = AbilitySystemComponent->GetNumericAttributeChecked(AttributeInspector.Attribute);
	float CompareValue;
	if (AttributeInspector.bUseOtherAttribute)
		CompareValue = AbilitySystemComponent->GetNumericAttributeChecked(AttributeInspector.OtherAttribute);	
	else
		CompareValue = AttributeInspector.Value;
	switch (AttributeInspector.Operation)
	{
	case EComparerOperation::Equal:
		return FMath::IsNearlyEqual(AttributeValue, CompareValue);
	case EComparerOperation::GreaterThan:
		return AttributeValue > CompareValue;
	case EComparerOperation::GreaterThanOrEqual:
		return AttributeValue >= CompareValue;
	case EComparerOperation::LessThan:
		return AttributeValue < CompareValue;
	case EComparerOperation::LessThanOrEqual:
		return AttributeValue <= CompareValue;
	case EComparerOperation::NotEqual:
		return AttributeValue != CompareValue;
	default:
		return false;
	}
}

DEFINE_FUNCTION(UActBlueprintFunctionLibrary::execArray_ContainsAll)
{
    // 1. 解析第一个参数 TargetArray
    Stack.MostRecentPropertyAddress = nullptr;
    Stack.StepCompiledIn<FArrayProperty>(NULL);
    void* TargetArrayAddr = Stack.MostRecentPropertyAddress;
    FArrayProperty* TargetArrayProp = CastField<FArrayProperty>(Stack.MostRecentProperty);

    // 2. 解析第二个参数 ItemsToFind
    Stack.MostRecentPropertyAddress = nullptr;
    Stack.StepCompiledIn<FArrayProperty>(NULL);
    void* ItemsToFindAddr = Stack.MostRecentPropertyAddress;
    FArrayProperty* ItemsToFindProp = CastField<FArrayProperty>(Stack.MostRecentProperty);

    P_FINISH; // 参数解析结束

    bool bReturnValue = true;

    // 安全检查：确保两个数组有效且内部元素类型一致
    if (TargetArrayAddr && ItemsToFindAddr && TargetArrayProp && ItemsToFindProp)
    {
        FScriptArrayHelper TargetHelper(TargetArrayProp, TargetArrayAddr);
        FScriptArrayHelper ItemsHelper(ItemsToFindProp, ItemsToFindAddr);

        int32 TargetNum = TargetHelper.Num();
        int32 ItemsNum = ItemsHelper.Num();

        // 如果要找的数组为空，逻辑上通常认为是被包含的
        if (ItemsNum > 0)
        {
            FProperty* InnerProp = TargetArrayProp->Inner;

            // 遍历待查找数组中的每一个元素
            for (int32 i = 0; i < ItemsNum; ++i)
            {
                uint8* ItemToFindPtr = ItemsHelper.GetRawPtr(i);
                bool bFoundCurrent = false;

                // 在目标数组中寻找该元素
                for (int32 j = 0; j < TargetNum; ++j)
                {
                    // 使用虚幻反射系统的 Identical 函数进行通用逻辑比较
                    if (InnerProp->Identical(ItemToFindPtr, TargetHelper.GetRawPtr(j)))
                    {
                        bFoundCurrent = true;
                        break;
                    }
                }

                // 只要有一个元素没找到，直接返回 false
                if (!bFoundCurrent)
                {
                    bReturnValue = false;
                    break;
                }
            }
        }
    }

    // 3. 设置返回值
    *(bool*)RESULT_PARAM = bReturnValue;
}


