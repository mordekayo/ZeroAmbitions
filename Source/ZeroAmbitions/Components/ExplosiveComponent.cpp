// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ExplosiveComponent.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


UExplosiveComponent::UExplosiveComponent()
{
	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	if (!this->IsTemplate())
	{
		Audio->SetupAttachment(this);
	}
}

void UExplosiveComponent::Explode(AController* Controller)
{
	TArray<AActor*> IgnoredActors;
	//IgnoredActors.Add(GetOwner());
	
	UGameplayStatics::ApplyRadialDamageWithFalloff(
		GetWorld(),
		MaxDamage,
		MinDamage,
		GetComponentLocation(),
		InnerRadius,
		OuterRadius,
		DamageFalloff,
		DamageTypeClass,
		IgnoredActors,
		GetOwner(),
		Controller,
		ECC_Visibility
		);
	
	if(IsValid(ExplosionVFX))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionVFX, GetComponentLocation());
	}
	
	if(OnExplosion.IsBound())
	{
		OnExplosion.Broadcast();
	}
	
	Audio->SetSound(FireSound);
	Audio->Play();
}
