// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/ZAAIBaseCharacter.h"

#include "Components/CharacterComponents/AIPatrollingComponent.h"

AZAAIBaseCharacter::AZAAIBaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIPatrollingComponent = CreateDefaultSubobject<UAIPatrollingComponent>(TEXT("AIPatrolling"));
}

UAIPatrollingComponent* AZAAIBaseCharacter::GetPatrollingComponent() const
{
	return AIPatrollingComponent;
}

UBehaviorTree* AZAAIBaseCharacter::GetBehaviorTree() const
{
	return BehaviorTree;
}
