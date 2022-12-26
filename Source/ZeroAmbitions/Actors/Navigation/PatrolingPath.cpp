// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Navigation/PatrolingPath.h"

const TArray<FVector>& APatrolingPath::GetWaypoints() const
{
	return WayPoints;
}
