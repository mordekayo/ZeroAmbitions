// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectiles/ExplosiveProjectile.h"

#include "Components/ExplosiveComponent.h"

AExplosiveProjectile::AExplosiveProjectile()
{
	ExplosiveComponent = CreateDefaultSubobject<UExplosiveComponent>(TEXT("ExplosiveComponent"));
	ExplosiveComponent->SetupAttachment(GetRootComponent());
}

void AExplosiveProjectile::OnProjectileLaunched()
{
	Super::OnProjectileLaunched();

	GetWorld()->GetTimerManager().SetTimer(DetonationTimer, this, &AExplosiveProjectile::OnDetonationTimerElapsed, DetonationDelay);
}

AController* AExplosiveProjectile::GetController() const
{
	const APawn* PawnOwner = Cast<APawn>(GetOwner());
	return IsValid(PawnOwner) ? PawnOwner->GetController() : nullptr;
}

void AExplosiveProjectile::OnDetonationTimerElapsed()
{
	ExplosiveComponent->Explode(GetController());
}
