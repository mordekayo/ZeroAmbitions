// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/BTService_Fire.h"
#include "AIController.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/ZABaseCharacter.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"

UBTService_Fire::UBTService_Fire()
{
	NodeName = "Fire";
}

void UBTService_Fire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const AAIController* AIController = OwnerComp.GetAIOwner();
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if(!IsValid(AIController) || !IsValid(Blackboard))
	{
		return;
	}

	AZABaseCharacter* Character = Cast<AZABaseCharacter>(AIController->GetPawn());
	if(!IsValid(Character))
	{
		return;
	}

	const UCharacterEquipmentComponent* EquipmentComponent = Character->GetCharacterEquipmentComponent();
	ARangeWeaponItem* RangeWeapon = EquipmentComponent->GetCurrentRangeWeapon();

	if(!IsValid(RangeWeapon))
	{
		return;
	}

	const AActor* CurrentTarget = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));

	if(!IsValid(CurrentTarget))
	{
		if(RangeWeapon->IsFiring())
		{
			RangeWeapon->StopFire();
		}
		return;
	}

	const float DistSq = FVector::DistSquared(CurrentTarget->GetActorLocation(), Character->GetActorLocation());
	if(DistSq > FMath::Square(MaxFireDistance))
	{
		Character->StopFire();
		return;
	}

	if(!(RangeWeapon->IsRealoading() || RangeWeapon->IsFiring()))
	{
		Character->StartFire();
	}
}
