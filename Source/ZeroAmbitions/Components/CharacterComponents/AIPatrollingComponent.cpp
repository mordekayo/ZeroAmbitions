// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterComponents/AIPatrollingComponent.h"
#include "Actors/Navigation/PatrolingPath.h"

bool UAIPatrollingComponent::CanPatrol()
{
	return IsValid(PatrolingPath) && PatrolingPath->GetWaypoints().Num() > 0;
}

FVector UAIPatrollingComponent::SelectClosestWaypoint()
{
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	const TArray<FVector> WayPoints = PatrolingPath->GetWaypoints();
	const FTransform PathTransform = PatrolingPath->GetActorTransform();

	FVector ClosestWayPoint;
	constexpr float MinSqDistance = FLT_MAX;
	for(int32 i = 0; i < WayPoints.Num(); ++i)
	{
		FVector WayPointWorld = PathTransform.TransformPosition(WayPoints[i]);
		const float CurrentSqDistance = (OwnerLocation - WayPointWorld).SizeSquared();
		if(CurrentSqDistance < MinSqDistance)
		{
			ClosestWayPoint = WayPointWorld;
			CurrentWayPointIndex = i;
		}
	}
	return ClosestWayPoint;
}

FVector UAIPatrollingComponent::SelectNextWaypoint()
{
	++CurrentWayPointIndex;
	const TArray<FVector> WayPoints = PatrolingPath->GetWaypoints();
	if(CurrentWayPointIndex == PatrolingPath->GetWaypoints().Num())
	{
		CurrentWayPointIndex = 0;
	}
	FTransform PathTransform = PatrolingPath->GetActorTransform();
	FVector WayPoint = PathTransform.TransformPosition(WayPoints[CurrentWayPointIndex]);
	return WayPoint;
}
