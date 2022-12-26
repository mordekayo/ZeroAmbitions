// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZAAIController.generated.h"

/**
 * 
 */
class UAISense;

UCLASS()
class ZEROAMBITIONS_API AZAAIController : public AAIController
{
	GENERATED_BODY()

public:
	AZAAIController();

protected:
	AActor* GetClosestSensedActor(TSubclassOf<UAISense> SensedClass) const;
};
