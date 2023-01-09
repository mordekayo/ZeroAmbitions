// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UHighlightInteractable;
/**
 * 
 */
UCLASS()
class ZEROAMBITIONS_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	class UAmmoWidget* GetAmmoWidget() const;
	class UItemsWidget* GetItemsWidget() const;

	void SetHighlightInteractibleVisibility(bool bIsVisible);

	void SetHighLightInteractibleActionText(FName KeyName);
	
protected:

	UFUNCTION(BlueprintCallable)
	ESlateVisibility IsWin() const;
	
	UFUNCTION(BlueprintCallable)
	ESlateVisibility IsAlive() const;

	UFUNCTION(BlueprintCallable)
	int GetPulse() const;
	
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable)
	float GetAdrenalinePercent() const;

	UFUNCTION(BlueprintCallable)
	FString GetAdrenalineState() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget names")
	FName AmmoWidgetName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget names")
	FName ItemsWidgetName;

	UPROPERTY(meta = (BindWidget))
	UHighlightInteractable* InteractableKey;
};
