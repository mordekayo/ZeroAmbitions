// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/PlayerHUDWidget.h"
#include "Characters/ZABaseCharacter.h"
#include "Components/CharacterComponents/CharacterAttributesComponent.h"

float UPlayerHUDWidget::GetHealthPercent() const
{
	float Result = 1.0f;
	APawn* Pawn = GetOwningPlayerPawn();
	AZABaseCharacter* BaseCharacter = Cast<AZABaseCharacter>(Pawn);
	if (IsValid(BaseCharacter))
	{
		const UCharacterAttributesComponent* CharacterAttributes = BaseCharacter->GetCharacterAttributesComponent();
		Result = CharacterAttributes->GetHealthPercent();
	}

	return Result;
}
