// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interactive/Pickables/PickableItem.h"
#include "PickablePowerUp.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ZEROAMBITIONS_API APickablePowerUp : public APickableItem
{
	GENERATED_BODY()
public:
	APickablePowerUp();
	virtual void Interact(AZABaseCharacter* Character) override;
	virtual FName GetActionEventName() const override;

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PowerUpMesh;
};
