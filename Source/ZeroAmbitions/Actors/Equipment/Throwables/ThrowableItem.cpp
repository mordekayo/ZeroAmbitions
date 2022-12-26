// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Equipment/Throwables/ThrowableItem.h"
#include "Characters/ZABaseCharacter.h"
#include "Actors/Projectiles/ZAProjectile.h"
#include "Components/CapsuleComponent.h"

void AThrowableItem::Throw()
{
	AZABaseCharacter* CharacterOwner = GetCharacterOwner();
	if(!IsValid(CharacterOwner))
	{
		return;
	}

	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
	if(!IsValid(Controller))
	{
		return;
	}

	FVector PlayerViewPoint = CharacterOwner->GetActorLocation() + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FVector ViewDirection = CharacterOwner->GetActorForwardVector();

	FVector SpawnLocation = PlayerViewPoint + ViewDirection * 100.0f;
	AZAProjectile* Projectile = GetWorld()->SpawnActor<AZAProjectile>(ProjectileClass, SpawnLocation, FRotator::ZeroRotator);

	if(IsValid(Projectile))
	{
		Projectile->SetOwner(GetOwner());
		Projectile->LaunchProjectile(ViewDirection);
	}
}
