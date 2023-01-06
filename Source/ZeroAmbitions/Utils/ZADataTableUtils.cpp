// Fill out your copyright notice in the Description page of Project Settings.

#include "ZADataTableUtils.h"
#include "Engine/DataTable.h"
#include "Inventory/Items/InventoryItem.h"

FWeaponTableRow* ZADataTableUtils::FindWeaponData(FName WeaponID)
{
	static const FString contextString(TEXT("Find Weapon Data"));

	const UDataTable* WeaponDataTable = nullptr;
		//LoadObject<UDataTable>(nullptr, TEXT("/Game/ZeroAmbitions/Core/Data/DataTables/DT_WeaponList.DT_WeaponList"));
	
	if(WeaponDataTable == nullptr)
	{
		return nullptr;
	}
	
	return WeaponDataTable->FindRow<FWeaponTableRow>(WeaponID, contextString);
}

FItemTableRow* ZADataTableUtils::FindInventoryItemData(const FName ItemID)
{
	static const FString contextString(TEXT("Find Item Data"));

	const UDataTable* InventoryDataTable = nullptr;
		//LoadObject<UDataTable>(nullptr, TEXT("/Game/ZeroAmbitions/Core/Data/DataTables/DT_InventoryItemList.DT_InventoryItemList"));
	
	if(InventoryDataTable == nullptr)
	{
		return nullptr;
	}
	
	return InventoryDataTable->FindRow<FItemTableRow>(ItemID, contextString);
}
