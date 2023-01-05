// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interactive/Pickables/PickablePowerUp.h"

#include "Inventory/Items/InventoryItem.h"
#include "Utils/ZADataTableUtils.h"

APickablePowerUp::APickablePowerUp()
{
	PowerUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerUpMesh"));
	SetRootComponent(PowerUpMesh);
}

void APickablePowerUp::Interact(AZABaseCharacter* Character)
{
	FItemTableRow* ItemData = ZADataTableUtils::FindInventoryItemData(GetDataTableID());

	if(ItemData == nullptr)
	{
		return;
	}

	TWeakObjectPtr<UInventoryItem> Item = TWeakObjectPtr<UInventoryItem>(NewObject<UInventoryItem>(Character, ItemData->InventoryItemClass));
	Item->Initialize(DataTableID, ItemData->InventoryItemDescription);

	const bool bPickedUp = Character->PickupItem(Item);

	if(bPickedUp)
	{
		Destroy();
	}
}

FName APickablePowerUp::GetActionEventName() const
{
	return ActionInteract;
}