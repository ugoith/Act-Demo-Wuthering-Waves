// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h" 
#include "ProceduralMeshComponent.h"
#include "Components/ActorComponent.h"
#include "HitAndDamageComponent.generated.h"

class UPayloadInfo;
class UGameplayAbility;
struct FGameplayTag;
class UGameplayEffect;

namespace EDrawDebugTrace
{
	enum Type : int;
}

UENUM(BlueprintType)
enum class ECollisionShapes:uint8
{
	Sphere = 0,
	Box = 1,
	Capsule = 2
};
USTRUCT(BlueprintType)
struct FShapeConfigInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ECollisionShapes CollisionShapes = ECollisionShapes::Sphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (ToolTip = "局部偏移"))
	FTransform OffsetTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="SphereCollisionConfig")
	float Radius = 400 ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CapsuleCollisionConfig")
	float CapsuleHalfHeight = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="CapsuleCollisionConfig")
	float CapsuleRadius = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoxCollisionConfig")
	FVector3f BoxExtent = FVector3f::ZeroVector;
};

USTRUCT(BlueprintType)
struct FApplyGEInfo : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly,meta = (Tooltip = "SetByCaller用"))
	FGameplayTag Tag;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (Tooltip = "SetByCaller用"))
	float Magnitude = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectToTarget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectToSelf;
};

USTRUCT(BlueprintType)
struct FPhysicConfig : public FTableRowBase
{
	GENERATED_BODY()
	FPhysicConfig():PhysicalActor(nullptr),LaunchVelocityMultiplier(FVector()){};
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (Tooltip = "当配置这个时，其他的力的配置选项作废"))
	TSubclassOf<AActor> PhysicalActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,meta = (Tooltip = "XYZ分别代表造成者至目标向量的前右上3个轴向乘数"))
	FVector LaunchVelocityMultiplier = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bXYOverride = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bZOverride = false;
	
	FORCEINLINE bool IsValid() const 
	{
		if ( PhysicalActor || LaunchVelocityMultiplier != FVector() ) return true;
		return false;
	}
};

USTRUCT(BlueprintType)
struct FDestructiveAttackConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bDestructive = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (Tooltip = "力的参照物,需要在ApprearanceInterface中进行配置,统一改为使用Start和End插槽"))
	FGameplayTag RefMesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FName StartSocket = FName();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FName EndSocket = FName();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta = (Tootip = "Start和End插槽中间插入的段数"))
	uint8 Sections = 0;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float ImpulseMagnitude = 0.f;
};

USTRUCT(BlueprintType)
struct FHitTestConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)                                
	TArray<FApplyGEInfo> ApplyGEConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly , meta = (Tooltip = "给敌人配置击飞用的"))
	FPhysicConfig PhysicConfig;
	UPROPERTY(EditAnywhere,BlueprintReadOnly , meta = (Tooltip = "配置物体破坏"))
	FDestructiveAttackConfig DestructiveConfig; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FShapeConfigInfo> CollisionConfigInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (ToolTip = "注释写在此处"))
	FString Describe;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttachBoneName;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannel;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AActor>> QueryActorClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIgnoreSelf = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDrawPreview = true;
};

UCLASS(ClassGroup=(Custom),Blueprintable,BlueprintType, meta=(BlueprintSpawnableComponent))
class ACT_API UHitAndDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHitAndDamageComponent();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<AActor*> HitActors;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = true))
	TArray<FHitResult>	FilterHitResults;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (Tooltip = "已经送达Event的Actors"))
	TArray<AActor*> SendGameplayEventActors;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPayloadInfo> PayloadInfo;
	
	UPROPERTY()
	TArray<TObjectPtr<UProceduralMeshComponent>> ProceduralMeshComponents;
	
	UFUNCTION(BlueprintCallable)
	void StartHitTest();
	UFUNCTION(BlueprintCallable)
	void UpdateHitTest(const FHitTestConfig& HitTestConfig);
	UFUNCTION(BlueprintCallable)
	void EndHitTest();
	void ResetHitTest();
	UFUNCTION()
	void SendHitSuccessEvent(const FHitResult& HitResult , const FHitTestConfig& HitTestConfig);
	
	UFUNCTION()
	void ApplySliceProceduralMesh(const FHitResult& HitResult,const FHitTestConfig& HitTestConfig);

	void UpdateForceDirection(const FHitTestConfig& HitTestConfig);
protected:
	// Called when the game starts
		FVector CurrentLocation = FVector::ZeroVector;
		FVector LastLocation = FVector::ZeroVector;
		FVector ForceDirection = FVector::ZeroVector;
		bool bFirstUpdate = true;
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
};
