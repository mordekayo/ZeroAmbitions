// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/Turret.h"

#include "Components/Weapon/WeaponBarellComponent.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* TurretRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TurretRoot"));
	SetRootComponent(TurretRoot);

	TurretBaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TurretBase"));
	TurretBaseComponent->SetupAttachment(TurretRoot);

	TurretBarellComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TurretBarell"));
	TurretBarellComponent->SetupAttachment(TurretBaseComponent);

	WeaponBarellComponent = CreateDefaultSubobject<UWeaponBarellComponent>(TEXT("BarellLogic"));
	WeaponBarellComponent->SetupAttachment(TurretBarellComponent);	
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (CurrentTurretState)
	{
		case ETurretState::Searching:
		{
			break;
		}
		case ETurretState::Firing:
		{
			break;
		}
	}
}

void ATurret::SetCurrentTarget(AActor* NewTarget)
{
	CurrentTarget = NewTarget;
	const ETurretState NewState = IsValid(CurrentTarget) ? ETurretState::Firing : ETurretState::Searching;
	SetCurrentTurretState(NewState);
}

void ATurret::SearchingMovement(float DeltaTime)
{
	FRotator TurretBaseRotation = TurretBaseComponent->GetRelativeRotation();
	TurretBaseRotation.Yaw += DeltaTime * BaseSearchingRotationRate;
	TurretBaseComponent->SetRelativeRotation(TurretBaseRotation);

	FRotator TurretBarellRotation = TurretBarellComponent->GetRelativeRotation();
	TurretBarellRotation.Pitch = FMath::FInterpTo(TurretBaseRotation.Pitch, 0.0f, DeltaTime, BarellPitchRotationRate);
	TurretBarellComponent->SetRelativeRotation(TurretBarellRotation);
}

void ATurret::FiringMovement(float DeltaTime)
{
	
}

void ATurret::SetCurrentTurretState(ETurretState NewState)
{
	CurrentTurretState = NewState;
}
