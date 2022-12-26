// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZeroAmbitionsTypes.h"
#include "Actors/Equipment/EquipableItem.h"
#include "CharacterEquipmentComponent.generated.h"

class AMeleeWeaponItem;
typedef TArray<AEquipableItem*, TInlineAllocator<static_cast<uint32>(EEquipmentSlots::MAX)>> TItemsArray;
typedef TArray<int32, TInlineAllocator<static_cast<uint32>(EAmmunitionType::MAX)>> TAmmunitionArray;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrentWeaponAmmoChanged, int32, int32)

class ARangeWeaponItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROAMBITIONS_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	EEquipableItemType GetCurrentEquippedItemType() const;

	ARangeWeaponItem* GetCurrentRangeWeapon() const;

	AMeleeWeaponItem* GetCurrentMeleeWeapon() const;
	bool IsEqupping() const;

	FOnCurrentWeaponAmmoChanged OnCurrentWeaponAmmoChangedEvent;

	void ReloadCurrentWeapon();
	void ReloadAmmoInCurrentWeapon(int32 NumberOfAmmo = 0, bool bCheckIfFull = false);
	void UnEquipCurrentItem();
	void AttachCurrentItemToEquippedSocket() const;
	void EquipItemInSlot(EEquipmentSlots Slot);

	void EquipNextItem();
	void EquipPreviousItem();

	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EAmmunitionType, int32> MaxAmmunitionAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EEquipmentSlots, TSubclassOf<class AEquipableItem>> ItemsLoadout;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loadout")
	EEquipmentSlots AutoEquipItemInSlot = EEquipmentSlots::None;
private:
	TAmmunitionArray AmmunitionArray;
	TItemsArray ItemsArray;

	bool bIsEquipping = false;
	UFUNCTION()
	void OnWeaponReloadComplete();
	void CreateLoadout();
	void AutoEquip();

	void EquipAnimationFinished();

	uint32 NextItemArraySlotIndex(uint32 CurrentSlotIndex) const;
	uint32 PreviousItemArraySlotIndex(uint32 CurrentSlotIndex) const;

	int32 GetAvailableAmmunitionForCurrentWeapon();
	UFUNCTION()
	void OnCurrentWeaponAmmoChanged(int32 NewAmmo);

	FDelegateHandle OnCurrentWeaponAmmoChangedHandle;
	FDelegateHandle OnCurrentWeaponReloadedHandle;
	
	EEquipmentSlots CurrentEquippedSlot;
	AEquipableItem* CurrentEquippedItem;
	ARangeWeaponItem* CurrentEquippedWeapon;
	AMeleeWeaponItem* CurrentMeleeWeaponItem;
	TWeakObjectPtr<class AZABaseCharacter> CachedBaseCharacter;

	FTimerHandle EquipTimer;
};
