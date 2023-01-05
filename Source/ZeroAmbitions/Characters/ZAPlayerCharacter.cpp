// Fill out your copyright notice in the Description page of Project Settings.

#include "ZAPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/MovementComponents/ZABaseCharacterMovementComponent.h"

AZAPlayerCharacter::AZAPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = 1;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	
	Team = ETeams::Player;
}

void AZAPlayerCharacter::MoveForward(float Value)
{
	if ((GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling()) && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector = YawRotator.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardVector, Value);
	}

	ZABaseCharacterMovementComponent->CacheForwardMovementInput(Value);
}

void AZAPlayerCharacter::MoveRight(float Value)
{
	if ((GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()-> IsFalling()) && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
		AddMovementInput(RightVector, Value);
	}
	ZABaseCharacterMovementComponent->CacheRightMovementInput(Value);
}

void AZAPlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AZAPlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AZAPlayerCharacter::TurnAtRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AZAPlayerCharacter::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AZAPlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	SpringArmComponent->TargetOffset += FVector(0.0f, 0.0f, HalfHeightAdjust);
}

void AZAPlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	SpringArmComponent->TargetOffset -= FVector(0.0f, 0.0f, HalfHeightAdjust);
}

void AZAPlayerCharacter::OnJumped_Implementation()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
}

bool AZAPlayerCharacter::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation();
}

