// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Items/PowerUps/MedKit.h"

#include "Components/CharacterComponents/CharacterAttributesComponent.h"

bool UMedKit::Consume(AZABaseCharacter* ConsumeTarget)
{
	UCharacterAttributesComponent* CharacterAttributesComponent = ConsumeTarget->GetCharacterAttributesComponent_Mutable();
	CharacterAttributesComponent->AddHealth(Health);
	return true;
}
