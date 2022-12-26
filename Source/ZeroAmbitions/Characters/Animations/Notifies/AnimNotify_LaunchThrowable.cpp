// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Animations/Notifies/AnimNotify_LaunchThrowable.h"

#include "Characters/ZABaseCharacter.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"

void UAnimNotify_LaunchThrowable::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	const AZABaseCharacter* CharacterOwner = Cast<AZABaseCharacter>(MeshComp->GetOwner());

	if(!IsValid(CharacterOwner))
	{
		return;
	}

	CharacterOwner->GetCharacterEquipmentComponent_Mutable()->LaunchCurrentThrowableItem();
}
