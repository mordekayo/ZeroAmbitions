// Fill out your copyright notice in the Description page of Project Settings.


#include "ZAPlayerController.h"
#include "../ZABaseCharacter.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "UI/Widgets/AmmoWidget.h"
#include "UI/Widgets/ItemsWidget.h"

void AZAPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBaseCharacter = Cast<AZABaseCharacter>(InPawn);
	if(IsLocalController() && CachedBaseCharacter.IsValid())
	{
		CreateAndInitizalizeWidgets();
		CachedBaseCharacter->OnInteractableObjectFound.BindUObject(this, &AZAPlayerController::OnInteractableObjectFound);
	}
	bShowMouseCursor = true;
}

bool AZAPlayerController::GetIgnoreCameraPitch() const
{
	return bIgnoreCameraPitch;
}

void AZAPlayerController::SetIgnoreCameraPitch(bool bIgnoreCameraPitch_In)
{
	bIgnoreCameraPitch = bIgnoreCameraPitch_In;
}

void AZAPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AZAPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AZAPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AZAPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AZAPlayerController::LookUp);
	InputComponent->BindAxis("TurnAtRate", this, &AZAPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUpAtRate", this, &AZAPlayerController::LookUpAtRate);
	
	InputComponent->BindAction("Jump",EInputEvent::IE_Pressed, this, &AZAPlayerController::Jump);
	InputComponent->BindAction("UnCrouch", EInputEvent::IE_Pressed, this, &AZAPlayerController::UnCrouch);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AZAPlayerController::ChangeCrouchState);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AZAPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AZAPlayerController::StopSprint);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AZAPlayerController::StartFire);
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AZAPlayerController::StopFire);
	InputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &AZAPlayerController::Reload);
	InputComponent->BindAction("NextItem", EInputEvent::IE_Pressed, this, &AZAPlayerController::NextItem);
	InputComponent->BindAction("PreviousItem", EInputEvent::IE_Pressed, this, &AZAPlayerController::PreviousItem);
	InputComponent->BindAction("EquipPrimaryItem", EInputEvent::IE_Pressed, this, &AZAPlayerController::EquipPrimaryItem);
	InputComponent->BindAction("PrimaryMeleeAttack", EInputEvent::IE_Pressed, this, &AZAPlayerController::PrimaryMeleeAttack);
	InputComponent->BindAction("SecondaryMeleeAttack", EInputEvent::IE_Pressed, this, &AZAPlayerController::SecondaryMeleeAttack);
	InputComponent->BindAction("UseInventory", EInputEvent::IE_Pressed, this, &AZAPlayerController::UseInventory);
	InputComponent->BindAction(ActionInteract, EInputEvent::IE_Pressed, this, &AZAPlayerController::Interact);
}

void AZAPlayerController::OnInteractableObjectFound(bool bIsFound, FName ActionName)
{
	if(!IsValid(PlayerHUDWidget))
	{
		return;
	}
	if(bIsFound == false)
	{
		PlayerHUDWidget->SetHighlightInteractibleVisibility(false);
		return;
	}

	TArray<FInputActionKeyMapping> ActionKeys = PlayerInput->GetKeysForAction(ActionName);
	const bool HasAnyKeys = ActionKeys.Num() != 0;
	if(HasAnyKeys)
	{
		const FName ActionKey = ActionKeys[0].Key.GetFName();
		PlayerHUDWidget->SetHighLightInteractibleActionText(ActionKey);
	}
	PlayerHUDWidget->SetHighlightInteractibleVisibility(HasAnyKeys);
}

void AZAPlayerController::MoveForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(Value);
	}
}

void AZAPlayerController::MoveRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(Value);
	}
}

void AZAPlayerController::Turn(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Turn(Value);
	}
}

void AZAPlayerController::LookUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUp(Value);
	}
}

void AZAPlayerController::TurnAtRate(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->TurnAtRate(Value);
	}
}

void AZAPlayerController::LookUpAtRate(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUpAtRate(Value);
	}
}

void AZAPlayerController::UnCrouch()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->UnCrouch();
	}
}
void AZAPlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Jump();
	}
}

void AZAPlayerController::ChangeCrouchState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeCrouchState();
	}
}

void AZAPlayerController::StartSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartSprint();
	}
}

void AZAPlayerController::StopSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopSprint();
	}
}

void AZAPlayerController::StartFire()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartFire();
	}
}

void AZAPlayerController::StopFire()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopFire();
	}
}

void AZAPlayerController::Reload()
{
	if(CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Reload();
	}
}

void AZAPlayerController::NextItem()
{
	if(CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->NextItem();
	}
}

void AZAPlayerController::PreviousItem()
{
	if(CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->PreviousItem();
	}
}

void AZAPlayerController::EquipPrimaryItem()
{
	if(CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->EquipPrimaryItem();
	}
}

void AZAPlayerController::PrimaryMeleeAttack()
{
	if(CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->PrimaryMeleeAttack();
	}
}

void AZAPlayerController::SecondaryMeleeAttack()
{
	if(CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SecondaryMeleeAttack();
	}
}

void AZAPlayerController::Interact()
{
	if(CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Interact();
	}
}

void AZAPlayerController::UseInventory()
{
	if(CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->UseInventory(this);
	}
}

void AZAPlayerController::CreateAndInitizalizeWidgets()
{
	if(!IsValid(PlayerHUDWidget))
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if(IsValid(PlayerHUDWidget))
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	if(CachedBaseCharacter.IsValid() && IsValid(PlayerHUDWidget))
	{
		UCharacterEquipmentComponent* CharacterEquipment = CachedBaseCharacter->GetCharacterEquipmentComponent_Mutable();
		UAmmoWidget* AmmoWidget = PlayerHUDWidget->GetAmmoWidget();
		if (IsValid(AmmoWidget))
		{
			CharacterEquipment->OnCurrentWeaponAmmoChangedEvent.AddUFunction(AmmoWidget, FName("UpdateAmmoCount"));
		}

		UItemsWidget* ItemsWidget = PlayerHUDWidget->GetItemsWidget();
		if (IsValid(ItemsWidget))
		{
			CharacterEquipment->OnCurrentItemAmountChanged.AddUFunction(ItemsWidget, FName("UpdateItemsCount"));
		}
		CharacterEquipment->UpdateAmmoAndItemsAmount();
	}
}
