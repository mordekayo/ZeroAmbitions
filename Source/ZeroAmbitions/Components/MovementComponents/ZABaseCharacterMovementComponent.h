// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZABaseCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZEROAMBITIONS_API UZABaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	void CacheForwardMovementInput(float MoveForwardInput);
	void CacheRightMovementInput(float MoveRigthInput);

	FORCEINLINE bool IsOutOfStamina() const { return bIsOutOfStamina; }
	void SetIsOutOfStamina(bool bIsOutOfStamina_In);
	
	void SetPussyState(bool bIsPussy_In);

	bool IsSprinting() const { return bIsSprinting; }
	virtual float GetMaxSpeed() const override;

	void StartSprint();
	void StopSprint();
	
	float GetProjectionToActor(const AActor* Actor, const FVector& CharacterLocation) const;

	virtual void PhysicsRotation(float DeltaTime) override;

	float GetCurrentMoveForwardInput();
	float GetCurrentMoveRightInput();

protected:

	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	virtual void PhysFalling(float DeltaTime, int32 Iterations) override;
	
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	
	class AZABaseCharacter* GetBaseCharacterOwner() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SprintSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float OutOfStaminaMaxSpeed = 150.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float PussySpeed = 200.0f;

private:

	float CurrentMoveForwardInput = 0.0f;
	float CurrentMoveRightInput = 0.0f;

	bool bIsSprinting;
	bool bIsOutOfStamina;
	bool bIsPussy = false;
	
	FRotator ForcedTargetRotation = FRotator::ZeroRotator;
	bool bForceRotation = false;
};
