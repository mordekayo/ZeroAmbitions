// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ZABaseCharacter.h"
#include "ZAAIBaseCharacter.generated.h"

/**
 * 
 */
class UAIPatrollingComponent;
UCLASS(Blueprintable)
class ZEROAMBITIONS_API AZAAIBaseCharacter : public AZABaseCharacter
{
	GENERATED_BODY()

public:
	AZAAIBaseCharacter(const FObjectInitializer& ObjectInitializer);

	UAIPatrollingComponent* GetPatrollingComponent() const;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPatrollingComponent* AIPatrollingComponent;
};
