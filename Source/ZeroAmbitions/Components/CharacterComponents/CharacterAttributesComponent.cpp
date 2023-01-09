// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributesComponent.h"
#include "../../Characters/ZABaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../../Subsystems/DebugSubsystem.h"
#include "Components/CapsuleComponent.h" 
#include "DrawDebugHelpers.h"
#include "../../ZeroAmbitionsTypes.h"
#include "Characters/ZAPlayerCharacter.h"
#include "Components/MovementComponents/ZABaseCharacterMovementComponent.h"

// Sets default values for this component's properties
UCharacterAttributesComponent::UCharacterAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCharacterAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(MaxHealth > 0.0f, TEXT("UCharacterAttributesComponent::BeginPlay max health cannot be equal to 0"));
	checkf(GetOwner()->IsA<AZABaseCharacter>(), TEXT("UCharacterAttributesComponent::BeginPlay() UCharacterAttributesComponent can be used only with AZABaseCharacter"))
	CachedBaseCharacterOwner = StaticCast<AZABaseCharacter*>(GetOwner());
	CachedBaseCharacterOwner->OnTakeAnyDamage.AddDynamic(this, &UCharacterAttributesComponent::OnTakeAnyDamage);

	Health = MaxHealth;
	Stamina = MaxStamina;
	Adrenaline = MaxAdrenaline / 2;
}

void UCharacterAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	DebugDrawAttributes();
#endif
	UpdateStaminaValue(DeltaTime);
	UpdateAdrenalineValue(DeltaTime);
}

float UCharacterAttributesComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float UCharacterAttributesComponent::GetAdrenalinePercent() const
{
	return Adrenaline / MaxAdrenaline;
}

FString UCharacterAttributesComponent::GetAdrenalineState() const
{
	switch (AdrenalineState)
	{
	case EAdrenalineState::Pussy:
		{
			return "PUSSY";
		}
	case EAdrenalineState::Brutal:
		{
			return "BRUTAL";
		}
	case EAdrenalineState::Psychopath:
		{
			return "PSYCHOPATH";
		}
	}

	return "ERROR";
}

void UCharacterAttributesComponent::AddHealth(float HealthToAdd)
{
	Health = FMath::Clamp(Health + HealthToAdd, 0.0f, MaxHealth);
	OnHealthChanged();
}

void UCharacterAttributesComponent::AddAdrenaline(float AdrenalineToAdd)
{
	Adrenaline = FMath::Clamp(Adrenaline + AdrenalineToAdd, 0.0f, MaxAdrenaline);
}

void UCharacterAttributesComponent::UpdateStaminaValue(float DeltaTime)
{
	if (CachedBaseCharacterOwner->GetBaseCharacterMovementComponent()->IsSprinting())
	{ 
		Stamina -= SprintStaminaConsumptionVelocity * DeltaTime;
		Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);
		if (Stamina <= 0)
		{
			if (OnOutOfStaminaEvent.IsBound())
			{
				OnOutOfStaminaEvent.Broadcast(false);
			}
		}
	}
	else
	{
		Stamina += StaminaRestoreVelocity * DeltaTime;
		Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);
		if (FMath::IsNearlyEqual(Stamina,MaxStamina))
		{
			if (OnOutOfStaminaEvent.IsBound())
			{
				OnOutOfStaminaEvent.Broadcast(true);
			}
		}
	}
}

void UCharacterAttributesComponent::UpdateAdrenalineValue(float DeltaTime)
{
	if (Adrenaline == 0)
	{
		AdrenalineState = EAdrenalineState::Pussy;
		Health = 0;
		OnHealthChanged();
	}
	else if(Adrenaline == MaxAdrenaline)
	{
		AdrenalineState = EAdrenalineState::Psychopath;
		Health = 0;
		OnHealthChanged();
	}
	else
	{
		if(Adrenaline > 0 && Adrenaline < MaxAdrenaline / 4)
		{
			AdrenalineState = EAdrenalineState::Pussy;
			CachedBaseCharacterOwner->GetBaseCharacterMovementComponent_Mutable()->SetPussyState(true);
		}
		if(Adrenaline > MaxAdrenaline / 4 && Adrenaline < MaxAdrenaline / 4 * 3)
		{
			AdrenalineState = EAdrenalineState::Brutal;
			CachedBaseCharacterOwner->GetBaseCharacterMovementComponent_Mutable()->SetPussyState(false);
		}
		if(Adrenaline > MaxAdrenaline / 4 * 3)
		{
			AdrenalineState = EAdrenalineState::Psychopath;
			CachedBaseCharacterOwner->GetBaseCharacterMovementComponent_Mutable()->SetPussyState(false);
		}
	}
	Adrenaline -= AdrenalineConsumptionVelocity * DeltaTime;
	Adrenaline = FMath::Clamp(Adrenaline, 0.0f, MaxAdrenaline);
}

void UCharacterAttributesComponent::DebugDrawAttributes()
{
	UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	if (!DebugSubsystem->IsCategoryEnabled(DebugCategoryCharacterAttributes))
	{
		return;
	}

	FVector HealthTextLocation = CachedBaseCharacterOwner->GetActorLocation() +
		(CachedBaseCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) * FVector::UpVector;
	DrawDebugString(GetWorld(), HealthTextLocation, FString::Printf(TEXT("Health: %.2f"), Health), nullptr, FColor::Green, 0.0f, true);	

	FVector StaminaTextLocation = CachedBaseCharacterOwner->GetActorLocation() +
		(CachedBaseCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 8.0f) * FVector::UpVector;
	DrawDebugString(GetWorld(), StaminaTextLocation, FString::Printf(TEXT("Stamina: %.2f"), Stamina), nullptr, FColor::Blue, 0.0f, true);
}

void UCharacterAttributesComponent::OnHealthChanged()
{
	if (Health <= 0.0f)
	{
		//UE_LOG(LogDamage, Warning, TEXT("UCharacterAttributesComponent::OnTakeAnyDamage character %s is killed by an actor %s"), *CachedBaseCharacterOwner->GetName(), *DamageCauser->GetName());
		if (OnDeathEvent.IsBound())
		{
			OnDeathEvent.Broadcast();
		}
	}
}

void UCharacterAttributesComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (!IsAlive())
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	UE_LOG(LogDamage, Warning, TEXT("UCharacterAttributesComponent::OnTakeAnyDamage %s received %.2f amount of damage from %s"), *CachedBaseCharacterOwner->GetName(), Damage, *DamageCauser->GetName());
	OnHealthChanged();
	
	auto InstigatorCharacter = Cast<AZAPlayerCharacter>(InstigatedBy->GetPawn());

	if(IsValid(InstigatorCharacter))
	{
		UCharacterAttributesComponent* CharacterAttributes = InstigatorCharacter->GetCharacterAttributesComponent_Mutable();
		CharacterAttributes->AddAdrenaline(Damage * 0.25);
	}
}



