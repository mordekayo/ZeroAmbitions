// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItem.h"
#include "MeleeWeaponItem.generated.h"

/**
 * 
 */

class UMeleeHitRegistrator;

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

	void SetIsHitRegistationEnabled(bool bIsRegistrationEnabled);
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee attack")
	TMap<EMeleeAttackTypes, FMeleeAttackDescription> Attacks;

	virtual void BeginPlay() override;
private:

	UFUNCTION()
	void ProcessHit(const FHitResult HitResult, const FVector& HitDirection);
	
	TArray<UMeleeHitRegistrator*> HitRegistrators;
	TSet<AActor*> HitActors;
	
	FMeleeAttackDescription* CurrentAttack;
	void OnAttackTimerElapsed();
	FTimerHandle AttackTimer;
};
