// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controllers/ZAAIController.h"

#include "Perception/AIPerceptionComponent.h"

AZAAIController::AZAAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

AActor* AZAAIController::GetClosestSensedActor(TSubclassOf<UAISense> SensedClass) const
{
	if (!IsValid(GetPawn()))
	{
		return nullptr;
	}
	TArray<AActor*> SensedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(SensedClass, SensedActors);

	AActor* ClosestActor = nullptr;
	float MinSquaredDistance = FLT_MAX;
	FVector PawnLocation = GetPawn()->GetActorLocation();

	for(AActor* SeenActor : SensedActors)
	{
		float CurrentSquaredDistance = (PawnLocation - SeenActor->GetActorLocation()).SizeSquared();
		if(CurrentSquaredDistance < MinSquaredDistance)
		{
			MinSquaredDistance = CurrentSquaredDistance;
			ClosestActor = SeenActor; 
		}
	}

	return ClosestActor;
}
