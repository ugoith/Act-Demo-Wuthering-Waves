// Fill out your copyright notice in the Description page of Project Settings.


#include "Act/Public/Character/PlayerCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AnimInstance/ActAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/ActHUD.h"
#include "Game/ActPlayerController.h"
#include "Game/ActPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
APlayerCharacterBase::APlayerCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	TeamID = 0;
	
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(GetCapsuleComponent());
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	CameraComponent->SetupAttachment(SpringArm);
	GetMesh()->SetRelativeLocation(FVector(0,0,-90));
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	SpringArm->SetupAttachment(GetRootComponent());
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	
}

// Called when the game starts or when spawned
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* APlayerCharacterBase::GetAbilitySystemComponent() const
{
	if (!GetPlayerState()) return nullptr;
	return GetPlayerState<AActPlayerState>()->GetAbilitySystemComponent();
}

void APlayerCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//客户端
	InitAbiltityActorInfo();
}

void APlayerCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!HasAuthority()) return;
	InitAbiltityActorInfo();
}

void APlayerCharacterBase::InitAbiltityActorInfo()
{
	AActPlayerState* ActPlayerState = GetPlayerState<AActPlayerState>();
	if (!ActPlayerState) return;
	UAbilitySystemComponent* ASC = ActPlayerState->AbilitySystemComponent;
	if (!ASC) return;
	ASC->InitAbilityActorInfo(ActPlayerState,this);
	AbilitySystemCompHasSet.Broadcast(ASC);
	AActPlayerController* PlayerController = Cast<AActPlayerController>(GetController());
	if (!PlayerController) return;
	InitializeEffectsToSelf();
	AActHUD* HUD = Cast<AActHUD>(PlayerController->GetHUD());
	if (!HUD) return;
	HUD->InitWidget(PlayerController,ActPlayerState,this,ASC,ActPlayerState->AttributeSet);
	
}

