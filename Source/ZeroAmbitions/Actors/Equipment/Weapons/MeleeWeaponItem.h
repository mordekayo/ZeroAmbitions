// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItem.h"
#include "MeleeWeaponItem.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMeleeAttackDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee attack")\
	TSubclassOf<class UDamageType> DamageTypeClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee attack")\
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee attack")\
	UAnimMontage* AttackMontage;
};

UCLASS(Blueprintable)
class ZEROAMBITIONS_API AMeleeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	AMeleeWeaponItem();

	void StartAttack(EMeleeAttackTypes AttackType);
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee attack")
	TMap<EMeleeAttackTypes, FMeleeAttackDescription> Attacks;

private:
	FMeleeAttackDescription* CurrentAttack;
	void OnAttackTimerElapsed();
	FTimerHandle AttackTimer;
};
