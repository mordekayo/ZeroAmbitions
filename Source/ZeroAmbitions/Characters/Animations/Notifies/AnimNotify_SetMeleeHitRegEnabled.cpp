// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Animations/Notifies/AnimNotify_SetMeleeHitRegEnabled.h"

#include "Actors/Equipment/Weapons/MeleeWeaponItem.h"
#include "Characters/ZABaseCharacter.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"

void UAnimNotify_SetMeleeHitRegEnabled::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AZABaseCharacter* CharacterOwner = Cast<AZABaseCharacter>(MeshComp->GetOwner());

	if(!IsValid(CharacterOwner))
	{
		return;
	}

	AMeleeWeaponItem* MeleeWeapon = CharacterOwner->GetCharacterEquipmentComponent()->GetCurrentMeleeWeapon();
	if(IsValid(MeleeWeapon))
	{
		MeleeWeapon->SetIsHitRegistationEnabled(bIsHitRegistrationEnabled);
	}
}
