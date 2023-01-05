// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/PlayerHUDWidget.h"

#include "AmmoWidget.h"
#include "HighlightInteractable.h"
#include "ItemsWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/ZABaseCharacter.h"
#include "Components/CharacterComponents/CharacterAttributesComponent.h"

UAmmoWidget* UPlayerHUDWidget::GetAmmoWidget() const
{
	return WidgetTree->FindWidget<UAmmoWidget>(AmmoWidgetName);
}

UItemsWidget* UPlayerHUDWidget::GetItemsWidget() const
{
	return WidgetTree->FindWidget<UItemsWidget>(ItemsWidgetName);
}

void UPlayerHUDWidget::SetHighlightInteractibleVisibility(bool bIsVisible)
{
	if(!IsValid(InteractableKey))
	{
		return;
	}
	
	if(bIsVisible)
	{
		InteractableKey->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractableKey->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerHUDWidget::SetHighLightInteractibleActionText(FName KeyName)
{
	if(IsValid(InteractableKey))
	{
		InteractableKey->SetActionText(KeyName);
	}
}

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
