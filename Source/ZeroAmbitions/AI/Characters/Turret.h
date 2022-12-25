// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Turret.generated.h"


UENUM(BlueprintType)
enum class ETurretState : uint8
{
	Searching,
	Firing
};

class UWeaponBarellComponent;

UCLASS()
class ZEROAMBITIONS_API ATurret : public APawn
{
	GENERATED_BODY()

public:

	ATurret();

	virtual void Tick(float DeltaTime) override;

	void SetCurrentTarget(AActor* NewTarget);

	virtual FVector GetPawnViewLocation() const override;

	virtual FRotator GetViewRotation() const override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* TurretBaseComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* TurretBarellComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWeaponBarellComponent* WeaponBarellComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float BaseSearchingRotationRate = 60.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float BaseFiringInterpSpeed = 60.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float BarellPitchRotationRate = 60.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxBarellPitchAngle = 60.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinBarrelPitchAngle = -30.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret parameters | Fire", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	float RateOfFire = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret parameters | Fire", meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 1.0f, UIMax = 1.0f))
	float BulletSpreadAngle = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parameters | Fire", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float FireDelayTime = 1.5f;
	
private:

	void SearchingMovement(float DeltaTime) const;
	void FiringMovement(float DeltaTime) const;
	
	void SetCurrentTurretState(ETurretState NewState);
	
	ETurretState CurrentTurretState = ETurretState::Searching;

	float GetFireInterval() const;
	FTimerHandle FireTimer;

	void MakeShot() const;
	
	AActor* CurrentTarget = nullptr;
};
