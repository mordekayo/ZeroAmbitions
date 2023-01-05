// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interactive/Pickables/PickableWeapon.h"

#include "ZeroAmbitionsTypes.h"
#include "Characters/ZABaseCharacter.h"
#include "Inventory/Items/InventoryItem.h"
#include "Utils/ZADataTableUtils.h"

APickableWeapon::APickableWeapon()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

void APickableWeapon::Interact(AZABaseCharacter* Character)
{
	FWeaponTableRow* WeaponRow = ZADataTableUtils::FindWeaponData(DataTableID);
	if(WeaponRow)
	{
		Character->AddEquipmentItem(WeaponRow->EquipableActor);
		Destroy();
	}
}

FName APickableWeapon::GetActionEventName() const
{
	return ActionInteract;
}
