// Fill out your copyright notice in the Description page of Project Settings.


#include "ZABaseCharacter.h"
// Fill out your copyright notice in the Description page of Project Settings.
#include "AIController.h"
#include "Components/MovementComponents/ZABaseCharacterMovementComponent.h"
#include "Components/CharacterComponents/CharacterAttributesComponent.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "Controllers/ZAPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZeroAmbitionsTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "Components/CapsuleComponent.h"

AZABaseCharacter::AZABaseCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UZABaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	ZABaseCharacterMovementComponent = StaticCast<UZABaseCharacterMovementComponent*>(GetCharacterMovement());

	GetMesh()->CastShadow = true;
	GetMesh()->bCastDynamicShadow = true;
	
	CharacterAttributesComponent = CreateDefaultSubobject<UCharacterAttributesComponent>(TEXT("CharacterAttributes"));
	CharacterEquipmentComponent = CreateDefaultSubobject<UCharacterEquipmentComponent>(TEXT("CharacterEquipment"));
}

void AZABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterAttributesComponent->OnDeathEvent.AddUObject(this, &AZABaseCharacter::OnDeath);
	CharacterAttributesComponent->OnOutOfStaminaEvent.AddUObject(this, &AZABaseCharacter::OnStaminaOutOrMax);
}

void AZABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryChangeSprintState(DeltaTime);
	
	if (bIKEnabled)
		UpdateIKSettings(DeltaTime);
}

void AZABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	const AAIController* AIController = Cast<AAIController>(NewController);
	if(IsValid(AIController))
	{
		FGenericTeamId TeamId(static_cast<uint8>(Team));
	}
	AZAPlayerController* PlayerController = Cast<AZAPlayerController>(NewController);
	if(IsValid(PlayerController))
	{
		ZAPlayerController = PlayerController;
	}
}

const UCharacterEquipmentComponent* AZABaseCharacter::GetCharacterEquipmentComponent() const
{
	return CharacterEquipmentComponent;
}

UCharacterEquipmentComponent* AZABaseCharacter::GetCharacterEquipmentComponent_Mutable() const
{
	return CharacterEquipmentComponent;
}

const UCharacterAttributesComponent* AZABaseCharacter::GetCharacterAttributesComponent() const
{
	return CharacterAttributesComponent;
}

UCharacterAttributesComponent* AZABaseCharacter::GetCharacterAttributesComponent_Mutable() const
{
	return CharacterAttributesComponent;
}

FGenericTeamId AZABaseCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(static_cast<uint8>(Team));
}

void AZABaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else if (CanCrouch())
	{
		Crouch();
	}
}

bool AZABaseCharacter::CanCrouch()
{
	return GetCharacterMovement()->IsMovingOnGround();
}

void AZABaseCharacter::StartSprint()
{
	bIsSprintRequested = true;
	if (bIsCrouched)
	{
		UnCrouch();
	}
}

void AZABaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

UZABaseCharacterMovementComponent* AZABaseCharacter::GetBaseCharacterMovementComponent_Mutable() const
{
	return ZABaseCharacterMovementComponent;
}

void AZABaseCharacter::OnSprintStart_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("OnSprintStart_Implementation()"))
}

void AZABaseCharacter::OnSprintEnd_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnSprintEnd_Implementation()"))
}

bool AZABaseCharacter::CanSprint()
{
	return GetCharacterMovement()->IsMovingOnGround() && !ZABaseCharacterMovementComponent->IsOutOfStamina() &&
		!ZABaseCharacterMovementComponent->IsCrouching();
}

void AZABaseCharacter::EnableRagdoll()
{
	GetMesh()->SetCollisionProfileName(CollisionProfileRagdoll);
	GetMesh()->SetSimulatePhysics(true);
}

void AZABaseCharacter::TryChangeSprintState(float DeltaTime)
{
	if (bIsSprintRequested && !ZABaseCharacterMovementComponent->IsSprinting() && CanSprint() && !FMath::IsNearlyZero(ZABaseCharacterMovementComponent->GetCurrentMoveForwardInput()))
	{
		ZABaseCharacterMovementComponent->StartSprint();
		OnSprintStart();
	}

	if (ZABaseCharacterMovementComponent->IsSprinting() && (FMath::IsNearlyZero(ZABaseCharacterMovementComponent->GetCurrentMoveForwardInput()) || !bIsSprintRequested || !CanSprint()))
	{
		ZABaseCharacterMovementComponent->StopSprint();
		OnSprintEnd();
	}
}

float AZABaseCharacter::CalculateIKOffsetForASocket(const FName& SocketName) const
{
	float ResultOffset = 0.f;

	const FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);
	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FVector TraceStart(SocketLocation.X, SocketLocation.Y, GetActorLocation().Z);
	FVector TraceEnd = TraceStart - (CapsuleHalfHeight + IKTraceDistance) * FVector::UpVector;

	FHitResult HitResult;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	const FVector FootSizeBox = FVector(1.f, 15.f, 7.f);
	if (UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceStart, TraceEnd, FootSizeBox, GetMesh()->GetSocketRotation(SocketName),
		TraceType, true, TArray<AActor*>(), EDrawDebugTrace::None, HitResult, true))
	{
		ResultOffset = TraceStart.Z - CapsuleHalfHeight - HitResult.Location.Z;
	}

	return ResultOffset;
}

void AZABaseCharacter::UpdateIKSettings(float DeltaSeconds)
{
	IKLeftFootOffset = FMath::FInterpTo(IKLeftFootOffset, CalculateIKOffsetForASocket(LeftFootSocketName), DeltaSeconds, IKInterpSpeed);
	IKRightFootOffset = FMath::FInterpTo(IKRightFootOffset, CalculateIKOffsetForASocket(RightFootSocketName), DeltaSeconds, IKInterpSpeed);
	IKPelvisOffset = FMath::FInterpTo(IKPelvisOffset, CalculateIKPelvisOffset(), DeltaSeconds, IKInterpSpeed);
}

float AZABaseCharacter::CalculateIKPelvisOffset() const
{
	return  IKRightFootOffset > IKLeftFootOffset ? -IKRightFootOffset : -IKLeftFootOffset;
}

void AZABaseCharacter::StartFire()
{
	if(CharacterEquipmentComponent->IsEqupping())
	{
		return;
	}
	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StartFire();
	}
}

void AZABaseCharacter::StopFire()
{
	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StopFire();
	}
}

void AZABaseCharacter::Reload()
{
	if(IsValid(CharacterEquipmentComponent->GetCurrentRangeWeapon()))
	{
		CharacterEquipmentComponent->ReloadCurrentWeapon();
	}
}

void AZABaseCharacter::NextItem()
{
	CharacterEquipmentComponent->EquipNextItem();
}

void AZABaseCharacter::PreviousItem()
{
	CharacterEquipmentComponent->EquipPreviousItem();
}

bool AZABaseCharacter::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() &&
		!ZABaseCharacterMovementComponent->IsCrouching() &&
		!ZABaseCharacterMovementComponent->IsOutOfStamina();
}

const UZABaseCharacterMovementComponent* AZABaseCharacter::GetBaseCharacterMovementComponent() const
{
	return ZABaseCharacterMovementComponent;
}

void AZABaseCharacter::OnDeath()
{
	float Duration = PlayAnimMontage(OnDeathAnimMontage);
	if (Duration == 0.0f)
	{
		EnableRagdoll();
	}
}

void AZABaseCharacter::OnStaminaOutOrMax(bool MaxOrOut)
{
	GetBaseCharacterMovementComponent_Mutable()->SetIsOutOfStamina(!MaxOrOut);
}

