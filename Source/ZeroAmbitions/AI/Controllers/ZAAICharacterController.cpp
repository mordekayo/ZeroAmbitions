// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controllers/ZAAICharacterController.h"
#include "AI/Characters/ZAAIBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CharacterComponents/AIPatrollingComponent.h"
#include "Perception/AISense_Sight.h"

void AZAAICharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if(IsValid(InPawn))
	{
		checkf(InPawn->IsA<AZAAIBaseCharacter>(), TEXT("AZAAICharacterController::SePawn ZAAICharacterController can posses only AZAAIBaseCharacters"));
		CachedAICharacter = StaticCast<AZAAIBaseCharacter*>(InPawn);
		RunBehaviorTree(CachedAICharacter->GetBehaviorTree());
	}
	else
	{
		CachedAICharacter = nullptr;
	}
}

void AZAAICharacterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
	if(!CachedAICharacter.IsValid())
	{
		return;
	}

	TryMoveToNextTarget();
}

void AZAAICharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if(!Result.IsSuccess())
	{
		return;
	}

	TryMoveToNextTarget();
}

void AZAAICharacterController::BeginPlay()
{
	Super::BeginPlay();

	UAIPatrollingComponent* PatrollingComponent = CachedAICharacter->GetPatrollingComponent();
	if(PatrollingComponent->CanPatrol())
	{
		const FVector ClosestWayPoint = PatrollingComponent->SelectClosestWaypoint();
		if(IsValid(Blackboard))
		{
			Blackboard->SetValueAsVector(BB_NextLocation, ClosestWayPoint);
			Blackboard->SetValueAsObject(BB_CurrentTarget, nullptr);
		}
		bIsPatrolling = true;
	}
}

void AZAAICharacterController::TryMoveToNextTarget()
{
	UAIPatrollingComponent* PatrollingComponent = CachedAICharacter->GetPatrollingComponent();
	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	if(IsValid(ClosestActor))
	{
		if(IsValid(Blackboard))
		{
			Blackboard->SetValueAsObject(BB_CurrentTarget, ClosestActor);
		}
		bIsPatrolling = false;
	}
	else if(PatrollingComponent->CanPatrol())
	{
		if(IsValid(Blackboard))
		{
			FVector WayPoint = bIsPatrolling ? PatrollingComponent->SelectNextWaypoint() : PatrollingComponent->SelectClosestWaypoint();
			Blackboard->SetValueAsVector(BB_NextLocation, WayPoint);
			Blackboard->SetValueAsObject(BB_CurrentTarget, nullptr);
		}
		bIsPatrolling = true;
	}
}

bool AZAAICharacterController::IsTargetReached(FVector TargetLocation) const
{
	return (TargetLocation - CachedAICharacter->GetActorLocation()).SizeSquared() <= FMath::Square(TargetReachedRadius);
}
