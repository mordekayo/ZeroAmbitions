// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponItem.h"
#include "Components/Weapon/WeaponBarellComponent.h"
#include "ZeroAmbitionsTypes.h"
#include "Characters/ZABaseCharacter.h"

ARangeWeaponItem::ARangeWeaponItem()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponBarell = CreateDefaultSubobject<UWeaponBarellComponent>(TEXT("WeaponBarell"));
	WeaponBarell->SetupAttachment(WeaponMesh, SocketBarellMuzzle);

	EquippedSocketName = SocketCharacterWeapon;
}

void ARangeWeaponItem::StartFire()
{
	if(bIsReloading)
	{
		return;	
	}
	MakeShot();

	if(WeaponFireMode == EWeaponFireMode::FullAuto)
	{ 
		GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
		GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &ARangeWeaponItem::MakeShot, GetShotTimerInterval(), true);
	}

}

void ARangeWeaponItem::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
}

void ARangeWeaponItem::StartReload()
{
	checkf(GetOwner()->IsA<AZABaseCharacter>(), TEXT("ARangeWeaponItem::StartReload() only character can be an owner of range weapon"));
	AZABaseCharacter* CharacterOwner = StaticCast<AZABaseCharacter*>(GetOwner());

	bIsReloading = true;
	if (IsValid(CharacterReloadMontage))
	{
		const float MontageDuration = CharacterOwner->PlayAnimMontage(CharacterReloadMontage);
		PlayAnimMontage(WeaponReloadMontage);
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, [this](){ EndReload(true); }, MontageDuration, false);
	}
	else
	{
		EndReload(true);
	}
}

void ARangeWeaponItem::EndReload(bool bIsSuccess)
{
	if(!bIsReloading)
	{
		return;
	}

	if(!bIsSuccess)
	{
		checkf(GetOwner()->IsA<AZABaseCharacter>(), TEXT("ARangeWeaponItem::StartReload() only character can be an owner of range weapon"));
		AZABaseCharacter* CharacterOwner = StaticCast<AZABaseCharacter*>(GetOwner());
		CharacterOwner->StopAnimMontage(CharacterReloadMontage);
		StopAnimMontage(WeaponReloadMontage, false);
	}
	bIsReloading = false;
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
	if(bIsSuccess && OnReloadComplete.IsBound())
	{
		OnReloadComplete.Broadcast();
	}
}

int32 ARangeWeaponItem::GetAmmo() const
{
	return Ammo;
}

int32 ARangeWeaponItem::GetMaxAmmo() const
{
	return MaxAmmo;
}

void ARangeWeaponItem::SetAmmo(int32 NewAmmo)
{
	Ammo = NewAmmo;
	if(OnAmmoChanged.IsBound())
	{
		OnAmmoChanged.Broadcast(Ammo);
	}
}

bool ARangeWeaponItem::CanShoot() const
{
	return Ammo > 0;	
}

EAmmunitionType ARangeWeaponItem::GetAmmoType() const
{
	return AmmoType;
}

FTransform ARangeWeaponItem::GetForeGripTransform() const
{
	return WeaponMesh->GetSocketTransform(SocketWeaponForeGrip);
}

void ARangeWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	SetAmmo(MaxAmmo);
}

void ARangeWeaponItem::MakeShot()
{
	checkf(GetOwner()->IsA<AZABaseCharacter>(), TEXT("ARangeWeaponItem::Fire() only character can be an owner of range weapon"));
	AZABaseCharacter* CharacterOwner = StaticCast<AZABaseCharacter*>(GetOwner());

	CharacterOwner->PlayAnimMontage(CharacterFireMontage);
	PlayAnimMontage(WeaponFireMontage);

	if(!CanShoot())
	{
		StopFire();
		if(Ammo == 0 && bAutoReloadEnabled)
		{
			CharacterOwner->Reload();
		}
		return;
	}

	EndReload(false);
	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
	if (!IsValid(Controller))
	{
		return;
	}

	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;

	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);

	FVector PlayerViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);

	WeaponBarell->Shot(PlayerViewPoint, PlayerViewDirection, Controller);
	SetAmmo(Ammo - 1);
}

float ARangeWeaponItem::GetShotTimerInterval()
{
	return 60.0f / RateOfFire;
}

float ARangeWeaponItem::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	float Result = 0.0f;
	if (IsValid(WeaponAnimInstance))
	{
		Result = WeaponAnimInstance->Montage_Play(AnimMontage);
	}
	return Result;
}

void ARangeWeaponItem::StopAnimMontage(const UAnimMontage* AnimMontage, float BlendOutTime) const
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	if (IsValid(WeaponAnimInstance))
	{
		WeaponAnimInstance->Montage_Stop(BlendOutTime, AnimMontage);
	}
}


