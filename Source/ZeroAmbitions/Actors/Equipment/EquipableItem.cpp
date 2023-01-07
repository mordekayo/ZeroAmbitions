#include "EquipableItem.h"

#include "Characters/ZABaseCharacter.h"

EEquipableItemType AEquipableItem::GetItemType() const
{
	return ItemType;
}

FName AEquipableItem::GetUnEquippedSocketName() const
{
	return UnEquippedSocketName;
}

FName AEquipableItem::GetEquippedSocketName() const
{
	return EquippedSocketName;
}

UAnimMontage* AEquipableItem::GetCharacterEquipAnimMontage() const
{
	return CharacterEquipAnimMontage;
}

void AEquipableItem::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	if(IsValid(NewOwner))
	{
		checkf(GetOwner()->IsA<AZABaseCharacter>(), TEXT("ARangeWeaponItem::StartReload() only character can be an owner of range weapon"));
		CachedCharacterOwner = StaticCast<AZABaseCharacter*>(GetOwner());
	}
	else
	{
		CachedCharacterOwner = nullptr;
	}
}

void AEquipableItem::Equip()
{
	if(OnEquipmentStateChanged.IsBound())
	{
		OnEquipmentStateChanged.Broadcast(true);
	}
}

void AEquipableItem::UnEquip()
{
	if(OnEquipmentStateChanged.IsBound())
	{
		OnEquipmentStateChanged.Broadcast(false);
	}
}

FName AEquipableItem::GetDataTableID() const
{
	return DataTableID;
}

bool AEquipableItem::IsSlotCompatible(EEquipmentSlots Slot)
{
	return CompatableEquipmentSlots.Contains(Slot);
}

AZABaseCharacter* AEquipableItem::GetCharacterOwner() const
{
	return CachedCharacterOwner.IsValid() ? CachedCharacterOwner.Get() : nullptr;
}
