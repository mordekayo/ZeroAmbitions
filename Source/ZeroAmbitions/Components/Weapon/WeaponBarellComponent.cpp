// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBarellComponent.h"
#include "ZeroAmbitionsTypes.h"
#include "DrawDebugHelpers.h"
#include "Subsystems/DebugSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Characters/ZAPlayerCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/CharacterComponents/CharacterAttributesComponent.h"

UWeaponBarellComponent::UWeaponBarellComponent()
{
	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	if (!this->IsTemplate())
	{
		Audio->SetupAttachment(this);
	}
}

void UWeaponBarellComponent::Shot(AController* Controller, float SpreadAngle)
{
	for (int i = 0; i < BulletsPerShot; ++i)
	{
		FVector MuzzleLocation = GetComponentLocation();
		FVector ShotStart = MuzzleLocation;
		FVector ShotDirection = GetForwardVector();
		ShotDirection += GetBulletSpreadOffset(FMath::RandRange(0.0f, SpreadAngle), ShotDirection.ToOrientationRotator());
		FVector ShotEnd = ShotStart + FiringRange * ShotDirection;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlashFX, MuzzleLocation, GetComponentRotation());

		UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
		bool bIsDebugEnabled = DebugSubsystem->IsCategoryEnabled(DebugCategoryRangeWeapon);

		FHitResult ShotResult;
		if (GetWorld()->LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Bullet))
		{
			ShotEnd = ShotResult.ImpactPoint;
		
			if (bIsDebugEnabled)
			{
				DrawDebugSphere(GetWorld(), ShotEnd, 10.0f, 24, FColor::Red, false, 1.0f);
			}
			AActor* HitActor = ShotResult.GetActor();
			if (IsValid(HitActor))
			{
				FPointDamageEvent DamageEvent;
				DamageEvent.HitInfo = ShotResult;
				DamageEvent.ShotDirection = ShotDirection;
				DamageEvent.DamageTypeClass = DamageTypeClass;
				float DistanceDamageCoef = 1.0f;
				if (IsValid(FalloffDiagram))
				{
					DistanceDamageCoef = FalloffDiagram->GetFloatValue((ShotEnd - MuzzleLocation).Size());
				}
				HitActor->TakeDamage(DamageAmount * DistanceDamageCoef, DamageEvent, Controller, GetOwner());
			}
		}

		UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BulletTraceFX, MuzzleLocation, GetComponentRotation());

		TraceFXComponent->SetVectorParameter(FXParamTraceEnd, ShotEnd);

		if (bIsDebugEnabled)
		{
			DrawDebugLine(GetWorld(), ShotStart, ShotEnd, FColor::Red, false, 1.0f, 0, 3.0f);
		}

		Audio->SetSound(FireSound);
		Audio->Play();
	}
}

FVector UWeaponBarellComponent::GetBulletSpreadOffset(float Angle, FRotator ShotRotation) const
{
	const float SpreadSize = FMath::Tan(Angle);
	const float RotationAngle = FMath::RandRange(0.0f, 2 * PI);

	const float SpreadY = FMath::Cos(RotationAngle);
	const float SpreadZ = FMath::Sin(RotationAngle);

	const FVector Result = (ShotRotation.RotateVector(FVector::UpVector) * SpreadZ
		+ ShotRotation.RotateVector(FVector::RightVector) * SpreadY) * SpreadSize;

	return Result;
}
