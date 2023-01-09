// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Items/InventoryItem.h"
#include "Adrenaline.generated.h"

/**
 * 
 */
UCLASS()
class ZEROAMBITIONS_API UAdrenaline : public UInventoryItem
{
	GENERATED_BODY()

public:
	virtual bool Consume(AZABaseCharacter* ConsumeTarget) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Medkit")
	float AdrenalineAmount = 25.0f;
};
