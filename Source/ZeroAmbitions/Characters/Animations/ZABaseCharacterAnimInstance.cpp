// Fill out your copyright notice in the Description page of Project Settings.


#include "ZABaseCharacterAnimInstance.h"
#include "Characters/ZABaseCharacter.h"
#include "Components/MovementComponents/ZABaseCharacterMovementComponent.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"

void UZABaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AZABaseCharacter>(), TEXT("UZABaseCharacterAnimInstance::NativeBeginPlay() UZABaseCharacterAnimInstance can be used only with AZABaseCharacter"));
	CachedBaseCharacter = StaticCast<AZABaseCharacter*>(TryGetPawnOwner());
}

void UZABaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}
	
	UZABaseCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetBaseCharacterMovementComponent_Mutable();
	Speed = CharacterMovement->Velocity.Size();
	bIsOutOfStamina = CharacterMovement->IsOutOfStamina();
	bIsFalling = CharacterMovement->IsFalling();
	bIsCrouch = CharacterMovement->IsCrouching();
	bIsSprinting = CharacterMovement->IsSprinting();
	bIsStrafing = !CharacterMovement->bOrientRotationToMovement;
	Direction = CalculateDirection(CharacterMovement->Velocity, CachedBaseCharacter->GetActorRotation());

	IKLeftFootOffset = FVector(-(CachedBaseCharacter->GetIKLeftFootOffset() + CachedBaseCharacter->GetIKPelvisOffset()), 0.0f, 0.0f);
	IKRightFootOffset = FVector(CachedBaseCharacter->GetIKRightFootOffset() + CachedBaseCharacter->GetIKPelvisOffset(), 0.0f, 0.0f);
	IKPelvisBoneOffset = FVector(0.0f, 0.0f, CachedBaseCharacter->GetIKPelvisOffset());

	AimRotation = CachedBaseCharacter->GetBaseAimRotation();
	const UCharacterEquipmentComponent* CharacterEquipment = CachedBaseCharacter->GetCharacterEquipmentComponent();
	CurrentEquippedItemType = CharacterEquipment->GetCurrentEquippedItemType();

	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipment->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		ForeGripSocketTransform = CurrentRangeWeapon->GetForeGripTransform();
	}
}
