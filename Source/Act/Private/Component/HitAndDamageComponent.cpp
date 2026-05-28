// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/HitAndDamageComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ActGameplayTags.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "AbilitySystem/Ability/HandleHitTest.h"
#include "BlueprintFunctionLibrary/ActBlueprintFunctionLibrary.h"
#include "Data/PayloadInfo.h"
#include "GameFramework/Character.h"
#include "Interface/AppearanceInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnitConversion.h"


class UProceduralMeshComponent;
// Sets default values for this component's properties
UHitAndDamageComponent::UHitAndDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PayloadInfo = NewObject<UPayloadInfo>();
	// ...
}


void UHitAndDamageComponent::StartHitTest()
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner());
	}
	if (!OwnerCharacter) return ;
	ResetHitTest();
}

void UHitAndDamageComponent::UpdateHitTest(const FHitTestConfig& HitTestConfig)
{
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter Is Invalid ! ! !"));
		return;
	}
	if (!OwnerCharacter->HasAuthority()) return ;
	//计算插槽各个方向的单位向量
	const FRotator& Rotation = OwnerCharacter->GetMesh()->GetSocketRotation(HitTestConfig.AttachBoneName);
	const FRotationMatrix& SocketRotation = FRotationMatrix(Rotation);
	const FVector& SocketForwardVector = SocketRotation.GetUnitAxis(EAxis::X);
	const FVector& SocketRightVector = SocketRotation.GetUnitAxis(EAxis::Y);
	const FVector& SocketUpVector = SocketRotation.GetUnitAxis(EAxis::Z);
	//仅查询Pawn类
	TArray<FHitResult> HitResults;
	FCollisionObjectQueryParams Params;
	Params.AddObjectTypesToQuery(ECC_Pawn);
	
	//不查询拥有者
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	if (HitTestConfig.bIgnoreSelf) 
		QueryParams.AddIgnoredActor(OwnerCharacter);
	QueryParams.bReturnPhysicalMaterial = false;
	
	bool bIsHit = false;
	//生成碰撞检测，得到HitResults结果
	for (const FShapeConfigInfo& CollisionConfig : HitTestConfig.CollisionConfigInfo)
	{
		const FVector& StartLocation = OwnerCharacter->GetMesh()->GetSocketLocation(HitTestConfig.AttachBoneName) + 
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
					CollisionShape.ShapeType = ECollisionShape::Box;
					CollisionShape.SetBox(CollisionConfig.BoxExtent);
					break;
				}
			case ECollisionShapes::Capsule:
				{
					CollisionShape.ShapeType = ECollisionShape::Capsule;
					CollisionShape.SetCapsule(CollisionConfig.CapsuleRadius,CollisionConfig.CapsuleHalfHeight);
					break;
				}
			case ECollisionShapes::Sphere:
				{
					CollisionShape.ShapeType = ECollisionShape::Sphere;
					CollisionShape.SetSphere(CollisionConfig.Radius);
					break;
				}
		}
		//配置的检测方式，只检测Pawn
		bIsHit = GetWorld()->SweepMultiByObjectType(HitResults,StartLocation,EndLocation,Rot,Params,CollisionShape,QueryParams) ? true : bIsHit;
	}
	//射线检测
	UpdateForceDirection(HitTestConfig);
	if (!bIsHit) return;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerCharacter);
	//过滤掉重复类和指定类
	for (const FHitResult& HitResult : HitResults)
	{
		SendHitSuccessEvent(HitResult,HitTestConfig);
		if (HitTestConfig.QueryActorClasses.IsEmpty())
		{
			if (!HitActors.Contains(HitResult.GetActor()))
			{
				HitActors.AddUnique(HitResult.GetActor());
				FilterHitResults.Add(HitResult);
			}
		}
		else
		{
			for (const TSubclassOf<AActor>& FilterClass : HitTestConfig.QueryActorClasses)
			{
				//是指定类的子类
				if (!HitResult.GetActor()->GetClass()->IsChildOf(FilterClass)) continue;
				//新碰撞到的Actor
				if (HitActors.Contains(HitResult.GetActor())) continue;
				HitActors.AddUnique(HitResult.GetActor());
				FilterHitResults.Add(HitResult);
			}
		}
		
	}
	
}


void UHitAndDamageComponent::EndHitTest()
{
	ResetHitTest();	
}

void UHitAndDamageComponent::ResetHitTest()
{
	HitActors.Empty();
	FilterHitResults.Empty();
	SendGameplayEventActors.Empty();
	ProceduralMeshComponents.Empty();
	bFirstUpdate = true;
	CurrentLocation = FVector::ZeroVector;
	LastLocation = FVector::ZeroVector;
	ForceDirection = FVector::ZeroVector;
	
}

void UHitAndDamageComponent::SendHitSuccessEvent(const FHitResult& HitResult, const FHitTestConfig& HitTestConfig)
{
	if (!HitResult.GetActor()) return;
	if (HitActors.Contains(HitResult.GetActor())) return;
	
	//发送命中事件
	FGameplayEventData PayloadData;
	PayloadData.EventTag= EventTags::EventTags_HitSuccess;
	PayloadData.Instigator = OwnerCharacter;
	PayloadData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
	
	if (!PayloadInfo)
	{
		PayloadInfo = NewObject<UPayloadInfo>();
	}
	if (!PayloadInfo) return;
	PayloadInfo->ApplyGEInfo = HitTestConfig.ApplyGEConfig;
	PayloadInfo->PhysicConfig = HitTestConfig.PhysicConfig;
	PayloadData.OptionalObject = PayloadInfo;
	//处理GE事件
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitResult.GetActor(),EventTags::EventTags_HitSuccess,PayloadData);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerCharacter,EventTags::EventTags_HitSuccess,PayloadData);
}

void UHitAndDamageComponent::ApplySliceProceduralMesh(const FHitResult& HitResult,const FHitTestConfig& HitTestConfig)
{

	if (UPrimitiveComponent* Comp = HitResult.GetComponent())
	{
		
		if (UProceduralMeshComponent* ProceduralMeshComp = Cast<UProceduralMeshComponent>(Comp))
		{
			if (ProceduralMeshComponents.Contains(Comp)) return;
			UProceduralMeshComponent* OtherComp = nullptr;
			IAppearanceInterface* Appearance = Cast<IAppearanceInterface>(OwnerCharacter);
			if (!Appearance) return;
			
			TMap<FGameplayTag,USkeletalMeshComponent*> SubMeshes;
			Appearance->Execute_GetSubMeshes(OwnerCharacter,SubMeshes);

			if (!SubMeshes.Num()) return;
			USkeletalMeshComponent* SceneComponent = *SubMeshes.Find(HitTestConfig.DestructiveConfig.RefMesh); 
			if (!SceneComponent) return;
			
			const FVector& WeaponDirection = SceneComponent->GetSocketLocation(HitTestConfig.DestructiveConfig.EndSocket) 
			- SceneComponent->GetSocketLocation(HitTestConfig.DestructiveConfig.StartSocket);
			//垂直于武器朝向与力的方向所成平面的向量
			const FVector& NormalVector = FVector::CrossProduct(ForceDirection , WeaponDirection);
			
			UKismetProceduralMeshLibrary::SliceProceduralMesh(ProceduralMeshComp,HitResult.Location,NormalVector,true
				,OtherComp,EProcMeshSliceCapOption::UseLastSectionForCap,nullptr);
			
			if (OtherComp)
			{
				ProceduralMeshComponents.AddUnique(OtherComp);
				ProceduralMeshComponents.AddUnique(ProceduralMeshComp);
				
				//const FVector& Impulse = ProceduralMeshComp->GetComponentLocation() - HitResult.Location;
				ProceduralMeshComp->AddRadialImpulse(HitResult.Location,500.f,HitTestConfig.DestructiveConfig.ImpulseMagnitude,RIF_Linear,true);
				//ProceduralMeshComp->AddImpulse(Impulse);
				OtherComp->SetCollisionObjectType(ECC_Destructible);
				OtherComp->SetSimulatePhysics(true);
				OtherComp->AddRadialImpulse(HitResult.Location,500.f,HitTestConfig.DestructiveConfig.ImpulseMagnitude,RIF_Linear,true);
				FGameplayEventData EventData;
				EventData.Instigator = OwnerCharacter;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerCharacter,EventTags::EventTags_SliceSuccess,EventData);
				//OtherComp->AddImpulse(-1.f * Impulse);
			}
		}
	}
	
}

void UHitAndDamageComponent::UpdateForceDirection(const FHitTestConfig& HitTestConfig)
{
	if (!OwnerCharacter) return;
	if (!HitTestConfig.DestructiveConfig.bDestructive || HitTestConfig.DestructiveConfig.RefMesh == FGameplayTag()) return;
	if (IAppearanceInterface* AppearanceInterface = Cast<IAppearanceInterface>(OwnerCharacter))
	{
		TMap<FGameplayTag,USkeletalMeshComponent*> SubMeshes;
		AppearanceInterface->Execute_GetSubMeshes(GetOwner(),SubMeshes);
		if (SubMeshes.IsEmpty()) return;
		USkeletalMeshComponent* MeshComponent = SubMeshes.FindRef(HitTestConfig.DestructiveConfig.RefMesh);
		if (!MeshComponent) return;
		//若为首次更新
		if (bFirstUpdate)
		{
			CurrentLocation = MeshComponent->GetSocketLocation(HitTestConfig.DestructiveConfig.StartSocket);
			LastLocation = CurrentLocation;
			bFirstUpdate = false;
		}
		//非首次更新
		LastLocation = CurrentLocation;
		CurrentLocation = MeshComponent->GetSocketLocation(HitTestConfig.DestructiveConfig.StartSocket);
		ForceDirection = CurrentLocation - LastLocation;
		
		FCollisionObjectQueryParams Params;
		Params.AddObjectTypesToQuery(ECC_Destructible);
		FCollisionShape CollisionShape;
		CollisionShape.ShapeType = ECollisionShape::Line;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerCharacter);
		QueryParams.bTraceComplex = false;
		TArray<FHitResult> HitResults;
		
		int i = -1;
		const FVector& StartToEnd = MeshComponent->GetSocketLocation(HitTestConfig.DestructiveConfig.EndSocket)
		- MeshComponent->GetSocketLocation(HitTestConfig.DestructiveConfig.StartSocket);
		const FVector& NormalStartToEnd = StartToEnd.GetSafeNormal();
		double DistancePerSection = HitTestConfig.DestructiveConfig.Sections ? StartToEnd.Length() / HitTestConfig.DestructiveConfig.Sections : StartToEnd.Length();
		bool bIsHit = false;
		//默认执行两次，若配置的数字大于1 则执行Sections + 1次
		while (i < HitTestConfig.DestructiveConfig.Sections || i <= 0)
		{
			if (HitTestConfig.bDebug)
			{
				UKismetSystemLibrary::DrawDebugLine(GetWorld(),LastLocation + NormalStartToEnd * (DistancePerSection * (i + 1)),
					CurrentLocation +  NormalStartToEnd * (DistancePerSection * (i + 1)),FLinearColor::Yellow,5.f);
				UKismetSystemLibrary::DrawDebugArrow(GetWorld(),LastLocation + NormalStartToEnd * (DistancePerSection * (i + 1)),
					CurrentLocation +  NormalStartToEnd * (DistancePerSection * (i + 1)),5.f,FLinearColor::Yellow,10.f);
			}
			
			bIsHit = GetWorld()->SweepMultiByObjectType(HitResults,LastLocation + NormalStartToEnd * (DistancePerSection * (i + 1)),
				 CurrentLocation + NormalStartToEnd * (DistancePerSection * (i + 1))
				,FQuat(),Params,CollisionShape,QueryParams);
			if (bIsHit)
			{
				ApplySliceProceduralMesh(HitResults[0],HitTestConfig);
			}
			
			++i;
		}
		
	}
}

// Called when the game starts
void UHitAndDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = UActBlueprintFunctionLibrary::GetOwnerCharacter(this);
	// ...
	
}


// Called every frame
void UHitAndDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

