// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/Widgets/PlayerHUDWidget.h"
#include "ZAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ZEROAMBITIONS_API AZAPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetPawn(APawn* InPawn) override;

	bool GetIgnoreCameraPitch() const;

	void SetIgnoreCameraPitch(bool bIgnoreCameraPitch_In);
protected:
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;
private:

	void OnInteractableObjectFound(bool bIsFound, FName ActionName);
	
	bool bIgnoreCameraPitch = false;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);
	void UnCrouch();
	void Jump();
	void ChangeCrouchState();
	void StartSprint();
	void StopSprint();
	
	void StartFire();
	void StopFire();
	void Reload();

	void NextItem();
	void PreviousItem();

	void EquipPrimaryItem();
	
	void PrimaryMeleeAttack();
	void SecondaryMeleeAttack();

	void Interact();
	void UseInventory();
	TSoftObjectPtr<class AZABaseCharacter> CachedBaseCharacter;

	void CreateAndInitizalizeWidgets();

	UPlayerHUDWidget* PlayerHUDWidget = nullptr;
	
};
