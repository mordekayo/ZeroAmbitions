// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemsWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZEROAMBITIONS_API UItemsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	int32 ItemsCount;

private:
	UFUNCTION()
	void UpdateItemsCount(int32 NewAmount);
};
