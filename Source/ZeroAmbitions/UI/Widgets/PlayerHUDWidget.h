// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

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
	protected:
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
	FName AmmoWidgetName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
	FName ItemsWidgetName;
	
};
