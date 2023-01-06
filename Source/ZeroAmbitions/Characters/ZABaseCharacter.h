// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "ZeroAmbitionsTypes.h"
#include "GameFramework/Character.h"
#include "UObject/ScriptInterface.h"
#include "ZABaseCharacter.generated.h"

class UInventoryItem;
class AEquipableItem;
DECLARE_DELEGATE_TwoParams(FOnInteractableObjectFound, bool, FName);

class UZABaseCharacterMovementComponent;
class UCharacterAttributesComponent;
class UCharacterEquipmentComponent;
class UCharacterInventoryComponent;
class IInteractable;
/**
 * 
 */
UCLASS()
class ZEROAMBITIONS_API AZABaseCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	
	AZABaseCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
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
	void Reload();

	void NextItem();
	void PreviousItem();

	void EquipPrimaryItem();

	void PrimaryMeleeAttack();
	void SecondaryMeleeAttack();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetIKLeftFootOffset() const { return IKLeftFootOffset;}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetIKRightFootOffset() const { return IKRightFootOffset;}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetIKPelvisOffset() const { return IKPelvisOffset; }

	virtual bool CanJumpInternal_Implementation() const;

	const UZABaseCharacterMovementComponent* GetBaseCharacterMovementComponent() const;
	UZABaseCharacterMovementComponent* GetBaseCharacterMovementComponent_Mutable() const;
	const UCharacterEquipmentComponent* GetCharacterEquipmentComponent() const;
	UCharacterEquipmentComponent* GetCharacterEquipmentComponent_Mutable() const;
	const UCharacterAttributesComponent* GetCharacterAttributesComponent() const;
	UCharacterAttributesComponent* GetCharacterAttributesComponent_Mutable() const;

	void Interact();

	void UseInventory(APlayerController* PlayerController);
	bool PickupItem(TWeakObjectPtr<UInventoryItem> ItemToPickup);
	void AddEquipmentItem(const TSubclassOf<AEquipableItem> EquipableItemClass) const;
	
	FOnInteractableObjectFound OnInteractableObjectFound;
	/** IGenericTeamAgentInterface*/
	virtual FGenericTeamId GetGenericTeamId() const override;
	/** ~IGenericTeamAgentInterface*/
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	class UCharacterInventoryComponent* CharacterInventoryComponent;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Team")
	ETeams Team = ETeams::Enemy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Team")
	float LineOfSightDistance = 500.0f;

	void TraceLineOfSight();

	UPROPERTY()
	TScriptInterface<IInteractable> LineOfSightObject;
	
	virtual void OnDeath();
	virtual void OnStaminaOutOrMax(bool MaxOrOut);

private:

	void RotateToCursor();
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


