// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterEquipmentComponent.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "Characters/ZABaseCharacter.h"
#include "ZeroAmbitionsTypes.h"
#include "Actors/Equipment/Throwables/ThrowableItem.h"
#include "Actors/Equipment/Weapons/MeleeWeaponItem.h"
#include "Math/UnitConversion.h"


void UCharacterEquipmentComponent::ReloadCurrentWeapon()
{
	check(IsValid(CurrentEquippedWeapon));
	if(GetAvailableAmmunitionForCurrentWeapon() <= 0)
	{
		return;
	}

	CurrentEquippedWeapon->StartReload();
}

void UCharacterEquipmentComponent::UnEquipCurrentItem()
{
	
	if(IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedItem->GetUnEquippedSocketName());
		CurrentEquippedItem->UnEquip();
	}
	if(IsValid(CurrentEquippedWeapon))
	{
		CurrentEquippedWeapon->StopFire();
		CurrentEquippedWeapon->EndReload(false);
		CurrentEquippedWeapon->OnAmmoChanged.Remove(OnCurrentWeaponAmmoChangedHandle);
		CurrentEquippedWeapon->OnReloadComplete.Remove(OnCurrentWeaponReloadedHandle);
	}
	PreviousEquippedSlot = CurrentEquippedSlot;
	CurrentEquippedSlot = EEquipmentSlots::None;
}

void UCharacterEquipmentComponent::AttachCurrentItemToEquippedSocket() const
{
	if(IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedItem->GetEquippedSocketName());
	}
}

void UCharacterEquipmentComponent::EquipItemInSlot(EEquipmentSlots Slot)
{
	if(bIsEquipping)
	{
		return;
	}
	
	UnEquipCurrentItem();
	
	CurrentEquippedItem = ItemsArray[static_cast<uint32>(Slot)];
	CurrentEquippedWeapon = Cast<ARangeWeaponItem>(CurrentEquippedItem);
	CurrentThrowableItem = Cast<AThrowableItem>(CurrentEquippedItem);
	CurrentMeleeWeaponItem = Cast<AMeleeWeaponItem>(CurrentEquippedItem);
	
	if(IsValid(CurrentEquippedItem))
	{
		UAnimMontage* EquipMontage = CurrentEquippedItem->GetCharacterEquipAnimMontage();
		if(IsValid(EquipMontage))
		{
			bIsEquipping = true;
			const float EquippingDuration = CachedBaseCharacter->PlayAnimMontage(EquipMontage);
			GetWorld()->GetTimerManager().SetTimer(EquipTimer, this, &UCharacterEquipmentComponent::EquipAnimationFinished, EquippingDuration, false);
		}
		else
		{
			AttachCurrentItemToEquippedSocket();
		}
		CurrentEquippedSlot = Slot;
		CurrentEquippedItem->Equip();
	}
	if(IsValid(CurrentEquippedWeapon))
	{
		OnCurrentWeaponAmmoChangedHandle = CurrentEquippedWeapon->OnAmmoChanged.AddUFunction(this, FName("OnCurrentWeaponAmmoChanged"));
		OnCurrentWeaponReloadedHandle = CurrentEquippedWeapon->OnReloadComplete.AddUFunction(this, FName("OnWeaponReloadComplete"));
		OnCurrentWeaponAmmoChanged(CurrentEquippedWeapon->GetAmmo());
	}
}

void UCharacterEquipmentComponent::EquipNextItem()
{
	const uint32 CurrentSlotIndex = static_cast<uint32>(CurrentEquippedSlot);
	uint32 NextSlotIndex = NextItemArraySlotIndex(CurrentSlotIndex);
	while(CurrentSlotIndex != NextSlotIndex &&
		IgnoreSlotsWhileSwitching.Contains(static_cast<EEquipmentSlots>(NextSlotIndex)) ||
		!IsValid(ItemsArray[NextSlotIndex]))
	{
		NextSlotIndex = NextItemArraySlotIndex(NextSlotIndex);
	}
	if (CurrentSlotIndex != NextSlotIndex)
	{
		EquipItemInSlot(static_cast<EEquipmentSlots>(NextSlotIndex));
	}
}

void UCharacterEquipmentComponent::EquipPreviousItem()
{
	const uint32 CurrentSlotIndex = static_cast<uint32>(CurrentEquippedSlot);
	uint32 PreviousSlotIndex = PreviousItemArraySlotIndex(CurrentSlotIndex);
	while(CurrentSlotIndex != PreviousSlotIndex &&
		IgnoreSlotsWhileSwitching.Contains(static_cast<EEquipmentSlots>(PreviousSlotIndex)) ||
		!IsValid(ItemsArray[PreviousSlotIndex]))
	{
		PreviousSlotIndex = PreviousItemArraySlotIndex(PreviousSlotIndex);
	}
	if (CurrentSlotIndex != PreviousSlotIndex)
	{
		EquipItemInSlot(static_cast<EEquipmentSlots>(PreviousSlotIndex));
	}
}

void UCharacterEquipmentComponent::LaunchCurrentThrowableItem()
{
	if(CurrentThrowableItem != nullptr)
	{
		CurrentThrowableItem->Throw();
		bIsEquipping = false;
		EquipItemInSlot(PreviousEquippedSlot);
	}
}

void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<AZABaseCharacter>(), TEXT("UCharacterEquipmentComponent::BeginPlay() UCharacterEquipmentComponent can be used only with base character"));
	CachedBaseCharacter = StaticCast<AZABaseCharacter*>(GetOwner());
	CreateLoadout();
	
	AutoEquip();
}

EEquipableItemType UCharacterEquipmentComponent::GetCurrentEquippedItemType() const
{
	EEquipableItemType Result = EEquipableItemType::None;
	if (IsValid(CurrentEquippedWeapon))
	{
		Result = CurrentEquippedWeapon->GetItemType();
	}
	return Result;
}

ARangeWeaponItem* UCharacterEquipmentComponent::GetCurrentRangeWeapon() const
{
	return CurrentEquippedWeapon;
}

AMeleeWeaponItem* UCharacterEquipmentComponent::GetCurrentMeleeWeapon() const
{
	return CurrentMeleeWeaponItem;
}

bool UCharacterEquipmentComponent::IsEqupping() const
{
	return bIsEquipping;
}

void UCharacterEquipmentComponent::OnWeaponReloadComplete()
{
	ReloadAmmoInCurrentWeapon();
}

void UCharacterEquipmentComponent::ReloadAmmoInCurrentWeapon(int32 NumberOfAmmo, bool bCheckIfFull)
{
	const int32 CurrentAmmo = CurrentEquippedWeapon->GetAmmo();
	const int32 AmmoToReload = CurrentEquippedWeapon->GetMaxAmmo() - CurrentEquippedWeapon->GetAmmo();
	int32 ReloadedAmmo = FMath::Min(GetAvailableAmmunitionForCurrentWeapon(), AmmoToReload);

	if(NumberOfAmmo > 0)
	{
		ReloadedAmmo = FMath::Min(ReloadedAmmo, NumberOfAmmo);
	}
	
	AmmunitionArray[static_cast<uint32>(CurrentEquippedWeapon->GetAmmoType())] -= ReloadedAmmo;
	CurrentEquippedWeapon->SetAmmo(ReloadedAmmo + CurrentAmmo);

	if(bCheckIfFull)
	{
		const bool bIsFullyReloaded = CurrentEquippedWeapon->GetAmmo() == CurrentEquippedWeapon->GetMaxAmmo();
		if(GetAvailableAmmunitionForCurrentWeapon() - ReloadedAmmo == 0 || bIsFullyReloaded)
		{
			CurrentEquippedWeapon->EndReload(true);
		}
	}
}

void UCharacterEquipmentComponent::CreateLoadout()
{
	AmmunitionArray.AddZeroed(static_cast<uint32>(EAmmunitionType::MAX));
	for(const TPair<EAmmunitionType, int32>& AmmoPair : MaxAmmunitionAmount)
	{
		AmmunitionArray[static_cast<uint32>(AmmoPair.Key)] = FMath::Max(AmmoPair.Value, 0);
	}

	ItemsArray.AddZeroed(static_cast<uint32>(EEquipmentSlots::MAX));
	for(const TPair<EEquipmentSlots, TSubclassOf<AEquipableItem>>& ItemPair : ItemsLoadout)
	{
		if(!IsValid(ItemPair.Value))
		{
			continue;
		}
		AEquipableItem* Item = GetWorld()->SpawnActor<AEquipableItem>(ItemPair.Value);
		Item->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Item->GetUnEquippedSocketName());
		Item->SetOwner(CachedBaseCharacter.Get());
		Item->UnEquip();
		ItemsArray[static_cast<uint32>(ItemPair.Key)] = Item;	
	}
}

void UCharacterEquipmentComponent::AutoEquip()
{
	if(AutoEquipItemInSlot != EEquipmentSlots::None)
	{
		EquipItemInSlot(AutoEquipItemInSlot);
	}
}

void UCharacterEquipmentComponent::EquipAnimationFinished()
{
	bIsEquipping = false;
	AttachCurrentItemToEquippedSocket();
}

uint32 UCharacterEquipmentComponent::NextItemArraySlotIndex(uint32 CurrentSlotIndex) const
{
	if(CurrentSlotIndex == ItemsArray.Num() - 1)
	{
		return 0;
	}
	else
	{
		return CurrentSlotIndex + 1;
	}
}

uint32 UCharacterEquipmentComponent::PreviousItemArraySlotIndex(uint32 CurrentSlotIndex) const
{
	if(CurrentSlotIndex == 0)
	{
		return ItemsArray.Num() - 1;
	}
	else
	{
		return CurrentSlotIndex - 1;
	}
}

int32 UCharacterEquipmentComponent::GetAvailableAmmunitionForCurrentWeapon()
{
	return AmmunitionArray[static_cast<uint32>(GetCurrentRangeWeapon()->GetAmmoType())];
}

void UCharacterEquipmentComponent::OnCurrentWeaponAmmoChanged(int32 NewAmmo)
{
	if(OnCurrentWeaponAmmoChangedEvent.IsBound())
	{
		OnCurrentWeaponAmmoChangedEvent.Broadcast(NewAmmo, GetAvailableAmmunitionForCurrentWeapon());
	}
}
