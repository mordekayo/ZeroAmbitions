// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZABaseCharacter.generated.h"

class UZABaseCharacterMovementComponent;
class UCharacterAttributesComponent;
class UCharacterEquipmentComponent;

/**
 * 
 */
UCLASS()
class ZEROAMBITIONS_API AZABaseCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	
	AZABaseCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void LookUp(float Value) {};
	virtual void TurnAtRate(float Value) {};
	virtual void LookUpAtRate(float Value) {};
	virtual void ChangeCrouchState();

	virtual void StartSprint();
	virtual void StopSprint();

	void StartFire();
	void StopFire();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetIKLeftFootOffset() const { return IKLeftFootOffset;}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetIKRightFootOffset() const { return IKRightFootOffset;}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetIKPelvisOffset() const { return IKPelvisOffset; }

	virtual bool CanJumpInternal_Implementation() const;

	UZABaseCharacterMovementComponent* GetBaseCharacterMovementComponent() const;
	const UCharacterEquipmentComponent* GetCharacterEquipmentComponent() const;
	const UCharacterAttributesComponent* GetCharacterAttributesComponent() const;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IK settings")
	bool bIKEnabled = false;

	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintStart();
	virtual void OnSprintStart_Implementation();

	UFUNCTION(BlueprintnativeEvent, Category = "Character | Movement")
	void OnSprintEnd();

	virtual void OnSprintEnd_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float StaminaRestoreVelocity = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float SprintStaminaConsumptionVelocity = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Controls")
	float BaseTurnRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Controls")
	float BaseLookUpRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float SprintSpeed = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IK settings")
	FName LeftFootSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IK settings")
	FName RightFootSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IK settings", meta = (ClampMin = 0.f, UIMin = 0.f))
	float IKTraceDistance = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IK settings", meta = (ClampMin = 0.f, UIMin = 0.f))
	float IKInterpSpeed = 30.0f;

	virtual bool CanSprint();

	virtual bool CanCrouch();
	
	UZABaseCharacterMovementComponent* ZABaseCharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	class UCharacterAttributesComponent* CharacterAttributesComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Animations")
	class UAnimMontage* OnDeathAnimMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	class UCharacterEquipmentComponent* CharacterEquipmentComponent;

	virtual void OnDeath();
	virtual void OnStaminaOutOrMax(bool MaxOrOut);
private:
	
	void EnableRagdoll();

	bool bIsSprintRequested = false;
	void TryChangeSprintState(float DeltaTime);
	float CalculateIKOffsetForASocket(const FName& SocketName) const;
	float CalculateIKPelvisOffset() const;
	void UpdateIKSettings(float DeltaSeconds);
	float IKLeftFootOffset = 0.0f;
	float IKRightFootOffset = 0.0f;
	float IKPelvisOffset = 0.0f;
	float CurrentStamina;

	TWeakObjectPtr<class AZAPlayerController> ZAPlayerController;
};
