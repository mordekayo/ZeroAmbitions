// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Items/PowerUps/Adrenaline.h"

#include "Components/CharacterComponents/CharacterAttributesComponent.h"

bool UAdrenaline::Consume(AZABaseCharacter* ConsumeTarget)
{
	UCharacterAttributesComponent* CharacterAttributesComponent = ConsumeTarget->GetCharacterAttributesComponent_Mutable();
	CharacterAttributesComponent->AddAdrenaline(AdrenalineAmount);
	return true;
}
