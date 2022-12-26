// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZAProjectile.generated.h"

UCLASS()
class ZEROAMBITIONS_API AZAProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AZAProjectile();

	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(FVector Direction);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	virtual void OnProjectileLaunched();
};
