// Fill out your copyright notice in the Description page of Project Settings.


#include "ZABaseCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "../../Characters/ZABaseCharacter.h"
#include "Kismet/GameplayStatics.h"

float UZABaseCharacterMovementComponent::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();
	if (bIsSprinting)
	{
		Result = SprintSpeed;
	}
	else if (bIsOutOfStamina)
	{
		Result = OutOfStaminaMaxSpeed;
	}
	return Result;
}

void UZABaseCharacterMovementComponent::SetIsOutOfStamina(bool bIsOutOfStamina_In)
{
	bIsOutOfStamina = bIsOutOfStamina_In;
}

void UZABaseCharacterMovementComponent::StartSprint()
{
	bIsSprinting = true;
	bForceMaxAccel = 1;
}

void UZABaseCharacterMovementComponent::StopSprint()
{
	bIsSprinting = false;
	bForceMaxAccel = 0;
}

float UZABaseCharacterMovementComponent::GetProjectionToActor(const AActor* Actor, const FVector& Location) const
{
	checkf(IsValid(Actor), TEXT("UZABaseCharacterMovementComponent::GetCharacterToActorProjection() cant be invoked then actor in NULL"));

	FVector ActorUpVector = Actor->GetActorUpVector();
	FVector ActorToCharacterDistance = Location - Actor->GetActorLocation();

	return FVector::DotProduct(ActorUpVector, ActorToCharacterDistance);
}

void UZABaseCharacterMovementComponent::PhysicsRotation(float DeltaTime)
{
	if (bForceRotation)
	{
		FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); // Normalized
		CurrentRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): CurrentRotation"));

		FRotator DeltaRot = GetDeltaRotation(DeltaTime);
		DeltaRot.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): GetDeltaRotation"));

		// Accumulate a desired new rotation.
		const float AngleTolerance = 1e-3f;

		if (!CurrentRotation.Equals(ForcedTargetRotation, AngleTolerance))
		{
			FRotator DesiredRotation = ForcedTargetRotation;
			// PITCH
			if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
			{
				DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
			}

			// YAW
			if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
			{
				DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
			}

			// ROLL
			if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
			{
				DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
			}

			// Set the new rotation.
			DesiredRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): DesiredRotation"));
			MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, /*bSweep*/ false);
		}
		else
		{
			ForcedTargetRotation = FRotator::ZeroRotator;
			bForceRotation = false;
		}
		return;
	}

	Super::PhysicsRotation(DeltaTime);
}

float UZABaseCharacterMovementComponent::GetCurrentMoveForwardInput()
{
	return CurrentMoveForwardInput;
}

float UZABaseCharacterMovementComponent::GetCurrentMoveRightInput()
{
	return CurrentMoveRightInput;
}

void UZABaseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	Super::PhysCustom(DeltaTime, Iterations);
}

void UZABaseCharacterMovementComponent::PhysFalling(float DeltaTime, int32 Iterations)
{
	Super::PhysFalling(DeltaTime, Iterations);
}

void UZABaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

AZABaseCharacter* UZABaseCharacterMovementComponent::GetBaseCharacterOwner() const
{
	return StaticCast<AZABaseCharacter*>(CharacterOwner);
}

void UZABaseCharacterMovementComponent::CacheForwardMovementInput(float MoveForwardInput)
{
	CurrentMoveForwardInput = MoveForwardInput;
}

void UZABaseCharacterMovementComponent::CacheRightMovementInput(float MoveRightInput)
{
	CurrentMoveRightInput = MoveRightInput;
}
