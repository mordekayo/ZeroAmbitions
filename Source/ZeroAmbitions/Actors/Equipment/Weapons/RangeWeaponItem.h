// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItem.h"
#include "RangeWeaponItem.generated.h"

DECLARE_MULTICAST_DELEGATE(FReloadComplete)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32)

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	Single,
	FullAuto
};

UENUM(BlueprintType)
enum class EReloadType : uint8
{
	FullClip,
	BulletByBullet
};

class UAnimMontage;

UCLASS()
class ZEROAMBITIONS_API ARangeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	ARangeWeaponItem();
	void StartFire();
	void StopFire();

	void StartReload();
	void EndReload(bool bIsSuccess);

	int32 GetAmmo() const;
	int32 GetMaxAmmo() const;
	void SetAmmo(int32 NewAmmo);
	bool CanShoot() const;
	EAmmunitionType GetAmmoType() const;
	FTransform GetForeGripTransform() const;

	FOnAmmoChanged OnAmmoChanged;
	FReloadComplete OnReloadComplete;
protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWeaponBarellComponent* WeaponBarell;

	//Fullclip reload type adds ammo only when the whole reload animations
	//BulletByBullet type requires section "ReloadEnd" in character reload animation 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Montage")
	EReloadType ReloadType = EReloadType::FullClip;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Montage")
	UAnimMontage* WeaponFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Montage")
	UAnimMontage* WeaponReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Montage")
	UAnimMontage* CharacterFireMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Montage")
	UAnimMontage* CharacterReloadMontage;

	// Rate of fire in rounds per minute
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	float RateOfFire = 600.0f;

	//Bullet spread half angle in degrees
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 2.0f, ClampMax = 2.0f))
	float SpreadAngle = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters")
	EWeaponFireMode WeaponFireMode = EWeaponFireMode::Single;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	EAmmunitionType AmmoType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	int32 MaxAmmo = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo")
	bool bAutoReloadEnabled = false;
private:

	float GetCurrentBulletSpreadAngle();

	bool bIsReloading = false;
	bool bIsFiring = false;
	
	int32 Ammo = 0;
	void MakeShot();
	float GetShotTimerInterval();
	void OnShotTimerElapsed();

	float PlayAnimMontage(UAnimMontage* AnimMontage);
	void StopAnimMontage(const UAnimMontage* AnimMontage, float BlendOutTime) const;

	FTimerHandle ShotTimer;
	FTimerHandle ReloadTimer;
};
