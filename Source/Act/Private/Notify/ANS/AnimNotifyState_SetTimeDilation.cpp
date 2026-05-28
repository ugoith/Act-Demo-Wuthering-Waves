// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/ANS/AnimNotifyState_SetTimeDilation.h"

#include "BlueprintFunctionLibrary/ActBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotifyState_SetTimeDilation::SetTimeDilation(UActorComponent* ActorComponent)
{
	AActor* Owner = ActorComponent->GetOwner();
	if (Owner == nullptr) return;
	switch (DilationType)
	{
	case ESetTimeDilationType::Self:
		{
			OriginalTimeDilation = Owner->GetOwner()->CustomTimeDilation;
			Owner->CustomTimeDilation = TimeDilation;
			break;
		}
	case ESetTimeDilationType::Global:
		{
			OriginalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(Owner);
			UGameplayStatics::SetGlobalTimeDilation(Owner, TimeDilation);
			break;
		}
	case ESetTimeDilationType::ExceptSelf:
		{
			OriginalTimeDilation = Owner->GetOwner()->CustomTimeDilation;
			TArray<FHitResult> HitResults;
			FCollisionObjectQueryParams ObjectParams;
			ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
			ObjectParams.AddObjectTypesToQuery(ECC_Destructible);
			ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
			ObjectParams.AddObjectTypesToQuery(ECC_PhysicsBody);
			ObjectParams.AddObjectTypesToQuery(ECC_Vehicle);
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(Owner);
			QueryParams.bTraceComplex = false;
			QueryParams.bDebugQuery = bDebug;
			bool bIsHit = GetWorld()->SweepMultiByObjectType(HitResults,Owner->GetActorLocation(),Owner->GetActorLocation(),FQuat(),
				ObjectParams,FCollisionShape::MakeSphere(ScopeOfAffection),QueryParams);
			if (!bIsHit) break;
			for (FHitResult& Hit : HitResults)
			{
				if (Hit.GetActor())
				{
					Hit.GetActor()->CustomTimeDilation = TimeDilation;
					AffectedActors.AddUnique(Hit.GetActor());
				}
			}
		}
	}
}

void UAnimNotifyState_SetTimeDilation::ResetTimeDilation(UActorComponent* ActorComponent)
{
	AActor* Owner = ActorComponent->GetOwner();
	if (Owner == nullptr) return;
	switch (DilationType)
	{
		case ESetTimeDilationType::Self:
			{
				Owner->CustomTimeDilation = OriginalTimeDilation;
				break;
			}
		case ESetTimeDilationType::Global:
			{
				UGameplayStatics::SetGlobalTimeDilation(Owner, OriginalTimeDilation);
				break;
			}
		case ESetTimeDilationType::ExceptSelf:
			{
				if (AffectedActors.Num() == 0) return;
				for (AActor* Actor : AffectedActors)
				{
					Actor->CustomTimeDilation = OriginalTimeDilation;
				}
				break;
			}
	}
	AffectedActors.Empty();
}

void UAnimNotifyState_SetTimeDilation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                   float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (UActBlueprintFunctionLibrary::QueryTargetActorOwnedTags(MeshComp->GetOwner(),QueryOwnedTags))
	{
		SetTimeDilation(MeshComp);
		Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	}
}

void UAnimNotifyState_SetTimeDilation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	ResetTimeDilation(MeshComp);
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
}
