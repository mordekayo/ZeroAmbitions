// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterEquipmentComponent.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "Characters/ZABaseCharacter.h"
#include "ZeroAmbitionsTypes.h"


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
	}
	if(IsValid(CurrentEquippedWeapon))
	{
		CurrentEquippedWeapon->StopFire();
		CurrentEquippedWeapon->EndReload(false);
		CurrentEquippedWeapon->OnAmmoChanged.Remove(OnCurrentWeaponAmmoChangedHandle);
		CurrentEquippedWeapon->OnReloadComplete.Remove(OnCurrentWeaponReloadedHandle);
	}

	CurrentEquippedSlot = EEquipmentSlots::None;
}

void UCharacterEquipmentComponent::AttachCurrentItemToEquippedSocket() const
{
	CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedItem->GetEquippedSocketName());
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
	while(CurrentSlotIndex !=  NextSlotIndex && !IsValid(ItemsArray[NextSlotIndex]))
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
	while(CurrentSlotIndex != PreviousSlotIndex && !IsValid(ItemsArray[PreviousSlotIndex]))
	{
		PreviousSlotIndex = PreviousItemArraySlotIndex(PreviousSlotIndex);
	}
	if (CurrentSlotIndex != PreviousSlotIndex)
	{
		EquipItemInSlot(static_cast<EEquipmentSlots>(PreviousSlotIndex));
	}
}

void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<AZABaseCharacter>(), TEXT("UCharacterEquipmentComponent::BeginPlay() UCharacterEquipmentComponent can be used only with base character"));
	CachedBaseCharacter = StaticCast<AZABaseCharacter*>(GetOwner());
	CreateLoadout();
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

bool UCharacterEquipmentComponent::IsEqupping() const
{
	return bIsEquipping;
}

void UCharacterEquipmentComponent::OnWeaponReloadComplete()
{
	const int32 CurrentAmmo = CurrentEquippedWeapon->GetAmmo();
	const int32 AmmoToReload = CurrentEquippedWeapon->GetMaxAmmo() - CurrentEquippedWeapon->GetAmmo();
	const int32 ReloadedAmmo = FMath::Min(GetAvailableAmmunitionForCurrentWeapon(), AmmoToReload);

	AmmunitionArray[static_cast<uint32>(CurrentEquippedWeapon->GetAmmoType())] -= ReloadedAmmo;
	CurrentEquippedWeapon->SetAmmo(ReloadedAmmo + CurrentAmmo);
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
		ItemsArray[static_cast<uint32>(ItemPair.Key)] = Item;	
	}
	
	// CurrentEquippedWeapon = GetWorld()->SpawnActor<ARangeWeaponItem>(SideArmClass);
	// CurrentEquippedWeapon->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketCharacterWeapon);
	// CurrentEquippedWeapon->SetOwner(CachedBaseCharacter.Get());
	// CurrentEquippedWeapon->OnAmmoChanged.AddUFunction(this, FName("OnCurrentWeaponAmmoChanged"));
	// CurrentEquippedWeapon->OnReloadComplete.AddUFunction(this, FName("OnWeaponReloadComplete"));
	// OnCurrentWeaponAmmoChanged(CurrentEquippedWeapon->GetAmmo());
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
