// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controllers/ZAAICharacterController.h"
#include "AI/Characters/ZAAIBaseCharacter.h"
#include "Components/CharacterComponents/AIPatrollingComponent.h"
#include "Perception/AISense_Sight.h"

void AZAAICharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if(IsValid(InPawn))
	{
		checkf(InPawn->IsA<AZAAIBaseCharacter>(), TEXT("AZAAICharacterController::SePawn ZAAICharacterController can posses only AZAAIBaseCharacters"));
		CachedAICharacter = StaticCast<AZAAIBaseCharacter*>(InPawn);
	}
	else
	{
		
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
		FVector ClosestWayPoint = PatrollingComponent->SelectClosestWaypoint();
		MoveToLocation(ClosestWayPoint);
		bIsPatrolling = true;
	}
}

void AZAAICharacterController::TryMoveToNextTarget()
{
	UAIPatrollingComponent* PatrollingComponent = CachedAICharacter->GetPatrollingComponent();
	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	if(IsValid(ClosestActor))
	{
		if(!IsTargetReached(ClosestActor->GetActorLocation()))
		{
			MoveToActor(ClosestActor);	
		}
		bIsPatrolling = false;
	}
	else if(PatrollingComponent->CanPatrol())
	{
		FVector WayPoint = bIsPatrolling ? PatrollingComponent->SelectNextWaypoint() : PatrollingComponent->SelectClosestWaypoint();
		if(!IsTargetReached(WayPoint))
		{
			MoveToLocation(WayPoint);
		}
		bIsPatrolling = true;
	}
}

bool AZAAICharacterController::IsTargetReached(FVector TargetLocation) const
{
	return (TargetLocation - CachedAICharacter->GetActorLocation()).SizeSquared() <= FMath::Square(TargetReachedRadius);
}
