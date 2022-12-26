// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectiles/ZAProjectile.h"
#include "ExplosiveProjectile.generated.h"

/**
 * 
 */

class UExplosiveComponent;
UCLASS()
class ZEROAMBITIONS_API AExplosiveProjectile : public AZAProjectile
{
	GENERATED_BODY()

public:
	AExplosiveProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UExplosiveComponent* ExplosiveComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion")
	float DetonationDelay = 2.0f;

	virtual void OnProjectileLaunched() override;
private:

	AController* GetController() const;
	void OnDetonationTimerElapsed();
	FTimerHandle DetonationTimer;
};
